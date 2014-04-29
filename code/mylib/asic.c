#include <stdlib.h>
#include "asic.h"
#include "lpc2106.h"
#include "spi.h"

#define U_RMS			1508
#define U_RMS_divide	100*65536

#define I_RMS			12
#define I_RMS_divide	10*65536

#define Wh_LSB			152//186 //313
#define Wh_multiply		1000000//10000000//1000000
#define UM_Wh			6579

//#define kWh_multiply	1000000000 

#define NO_LOAD_I			0.04 //40mA
//#define NO_LOAD_I_multiply	1000 //100mA

#define ASIC_DELAY	78
 
#define ChipSelect		10
#define RESET_pin		11
#define IRQ_pin			12

static unsigned long ActivEnergy, ActivEnergy_W; //Energia activa
static unsigned char asic_status, zero_cross, watt_inc, start_meter, energy_count;
static unsigned long initial;
static unsigned int isr_code, energy_start;


static float Urms, Irms, Pfactor;
//static int teste[10], var;

inline void asic_delay(int delay); // funcao que executa um delay de: delay x 51.7ns

static Spi_device spi;
//static unsigned int interrupt_register = 0x00;

static void cs_set(int value){
	if(value)
		IOCLR = 1<<ChipSelect;
	else
		IOSET = 1<<ChipSelect;
}

/* Inicializacao dos portos GPIO, SPI e Modo de acumulacao de energia positiva
 */
void asic_init() {
	
	unsigned int mode = 0x800C; // POAM Activo
	unsigned char t[3];
	
	/* direcionar os pinos de CS, RESET e IRQ para o GPIO */
	uint32_t pinsel = PINSEL0;
	PINSEL0 = pinsel & ~(3 << ChipSelect * 2) & ~(3 << RESET_pin * 2) & ~(3 << IRQ_pin * 2);
	/* desactivar os sinais; são todos active low */
	IOSET = 1 << ChipSelect | 1 << RESET_pin;
	/* e programá-los como saídas excepto IRQ*/
	uint32_t iodir = IODIR;
	IODIR = iodir | 1 << ChipSelect | 1 << RESET_pin & ~(1 << IRQ_pin);

	
	//spi.clock = 10;
	
	spi.cpha = 0;
	spi.cpol = 0;
	spi.cbits = 8;
	spi.lsbf = 0; // MSB first
	spi.cs = cs_set;
	
	/*spi = {
		10,					//SCLK
		0,					//CPHA
		0,					//CPOL
		9,					//Numero de bits por palavra
		0,					//LSBF
		cs_set				
	};*/
	t[0] = 0x09; //Mode Register
	t[1] = 0x80; //POAM Activo
	t[2] = 0x0C;
	write_asic(t);
	
}

/* Escrita no ASIC
 * Parametros de Entrada: endereco de uma array de 3 posicoes: end DATA1 DATA0 
 */
void write_asic(unsigned char *command) {
	uint8_t out_buffer;
	
	out_buffer = *command++ & 0x3F;
	out_buffer |= 0x80; //10xx xxxx
	//out_buffer[1] = *command++;
	//out_buffer[2] |= *command; //10xx xxxx
	spi_transaction_begin(&spi);
	
	spi_transaction_transfer(&spi, 1, &out_buffer, 0x00);
	out_buffer = *command++;
	asic_delay( ASIC_DELAY );// t7
	spi_transaction_transfer(&spi, 1, &out_buffer, 0x00);
	out_buffer = *command;
	chrono_delay( 1 ); //t6
	spi_transaction_transfer(&spi, 1, &out_buffer, 0x00);
	spi_transaction_end(&spi);

}


/* Ler do ASIC
 * Parametros de Entrada: endereco
 * 					      Dimensao
 */
void read_asic(unsigned char *command, unsigned char dim){
	uint8_t out_buffer;
	
	out_buffer = *command & 0x3F;
	spi_transaction_begin(&spi);
	spi_transaction_transfer(&spi, 1, &out_buffer, 0x00);
	chrono_delay( 1 ); //t9
	while(dim--){
		spi_transaction_transfer(&spi, 1, 0x00, command++);
		asic_delay( ASIC_DELAY );// t10
	}
	spi_transaction_end(&spi);
}

/* Verificar existencia de interrupt
 */
unsigned int irq_asic(){
	int state_pin = IOPIN;
	if( (state_pin>>IRQ_pin) & 0x01 ) return 0;
	return 1;
}

/* Reset
 */	
void reset_asic(){
	IOCLR = 1<<RESET_pin;
	chrono_delay(10);
	IOSET = 1<<RESET_pin;
	chrono_delay(10);
}

/* Desinibir Interrupts
 * Parametros de Entrada: id do interrupt
 */
void enableIsr_asic( unsigned char command ){
	unsigned char t[3];
	unsigned int interrupt_register = 0x00;
	
	t[0] = 0x0A; // Interrupt Enable Register
	read_asic( t, 2); //Ler 
	interrupt_register = t[0];
	interrupt_register <<= 8;
	interrupt_register |= t[1];
	
	interrupt_register |= (1<<command); 
	
	t[0] = 0x0A;// Enable Status Register Address
	t[1] = interrupt_register >> 8;
	t[2] = (interrupt_register & 0xFF);
	write_asic(t);
}

/* Inibir interrupts
 * Parametros de Entrada: id do interrupt
 */
void disableIsr_asic( unsigned char command ){
	unsigned char t[3];
	unsigned int interrupt_register = 0x00;
	
	t[0] = 0x0A; // Interrupt Enable Register
	read_asic( t, 2); //Ler 
	interrupt_register = t[0];
	interrupt_register <<= 8;
	interrupt_register |= t[1];
	
	interrupt_register &= ~( 1<<command ); 
	
	t[0] = 0x0A;// Enable Status Register Address
	t[1] = interrupt_register >> 8;
	t[2] = interrupt_register & 0x00FF;
	write_asic(t);
}

/* Identificar origem do interrupt
 */
unsigned int isrStatus_asic(){
	unsigned char t[2];
	unsigned int tmp = 0x00;
	
	t[0] = 0x0C; // Reset Interrupt Status Register
	read_asic(t, 2);
	tmp = t[0];
	tmp <<= 8;
	tmp |= t[1];
	
	t[0] = 0;
	while( t[0]<16 ){
		if( tmp&0x01 )
			return t[0];
	tmp >>= 1;
	++t[0];
	}
	return 16;
}

//Calibrar Urms e Irms
/*
unsigned int calibrate(){
	
	int u, i = 3;
	int u_arr[3];
	unsigned char t[3];
	
	enableIsr_asic( ZX );
	isrStatus_asic();
	
	while(i){
		if( irq_asic() ){
			//if( isrStatus_asic() == ZX ){
				t[0] = 0x17;
				read_asic(t, 3);
				u = t[0];
				u <<= 8;
				u |= t[1];
				u <<= 8;
				u |= t[2];
				u_arr[i-1] = u;
				isrStatus_asic();
				--i;
			//}
		}
	}
	
	return (u_arr[0]+u_arr[1]+u_arr[2])/3;
}
*/		
	


/* Calcular Urms
*/
float Urms_asic(){
	unsigned char t[3];
	unsigned int tmp;
	float x;

	t[0] = 0x17;
	read_asic(t, 3);
	tmp = t[0];
	tmp <<= 8;
	tmp |= t[1];
	tmp <<= 8;
	tmp |= t[2];
	
	x = U_RMS_divide;
	x = tmp * U_RMS;// / U_RMS_divide;
	x /= U_RMS_divide;	
	//x = tmp * U_RMS;
	//x /= Exponente;
	return x;
}

/* Calcular Irms
*/
float Irms_asic(){
	unsigned char t[3];
	unsigned int tmp;
	float x;

	t[0] = 0x16;
	read_asic(t, 3);
	tmp = t[0];
	tmp <<= 8;
	tmp |= t[1];
	tmp <<= 8;
	tmp |= t[2];
	
	
	x = tmp * I_RMS; 
	x /= I_RMS_divide;

	return x;
}
/* Calculo da energia activa
 */
unsigned int ActiveEnergy_asic(){
	unsigned char t[3];
	unsigned int tmp, tmp_apparent;
	//float x;
		
	
		t[0] = 0x03;
		read_asic(t, 3);
		tmp = t[0];
		tmp <<= 8;
		tmp |= t[1];
		tmp <<= 8;
		tmp |= t[2];
	
		if( t[0]>>7 ) //Se o valor for negativo
			return 0;
		
		/*t[0] = 0x06;
		read_asic(t, 3);
		tmp_apparent = t[0];
		tmp_apparent <<= 8;
		tmp_apparent |= t[1];
		tmp_apparent <<= 8;
		tmp_apparent |= t[2];
	
		Pfactor = tmp/1.0;
		Pfactor /= (tmp_apparent/0.848);*/

	/*if( Irms < NO_LOAD_I )// <100mA não existe carga
		ActivEnergy = Pfactor = 0;
	else
		ActivEnergy += tmp;

	x = ActivEnergy * kWh_LSB;
	x /= kWh_multiply;*/
	return tmp;
		
}



float Urms_value(){ return Urms; }
float Irms_value(){	return Irms; }
float PowerFactor(){ return Pfactor; }
float ActiveE_value(){
	float x;
	
	x = ActivEnergy * Wh_LSB;
	x /= Wh_multiply;
	
	return x;
}
		
void startRMS_measure_asic(){
	asic_status = START;
}

unsigned int isWattInc(){
	if( watt_inc ){
		watt_inc = OFF;
		return 1;
	}
	return 0;
}
		
unsigned int measureRMS_complete_asic(){
	if( asic_status==COMPLETE)		
		return 1;
	return 0;
}

/*	
void endRMS_measure_asic(){
	asic_status = INIT;
}*/
	
//maquina de estados RMS
void asic_rms(){
	char tmp;
	switch(asic_status){
		case INIT:
			break;
		case START:
			initial = chrono_start();
			asic_status = MEASURE;
			break;
		case MEASURE:
			if( chrono_timeout(initial, SETTLING_TIME) ){
				enableIsr_asic( ZX );
				enableIsr_asic( ZXTO );
				//isrStatus_asic();
				asic_status = WAIT_ISR;
			}
			break;
		case WAIT_ISR:
			if( irq_asic() )
				asic_status = RMS;
			break;
		case RMS:
			isr_code = isrStatus_asic();
			if( isr_code==ZX ){ // zero crossings
				if( ++zero_cross==3 ){
					zero_cross = 0;
					Urms = Urms_asic();
					Irms = Irms_asic();
					asic_status = COMPLETE;
					if( Irms<=NO_LOAD_I && start_meter==OFF){
						Irms = 0;
						ActiveEnergy_asic();
					}
					else
						if( Irms>NO_LOAD_I && start_meter==OFF){
							start_meter = ON;
							energy_count = 0;
							ActivEnergy_W += ActiveEnergy_asic();
							energy_start = chrono_start();
						}
						else
							if( Irms<=NO_LOAD_I && start_meter==ON ){
								ActivEnergy_W += ActiveEnergy_asic();
								start_meter = OFF;
								asic_status = ENERGY;
							}
							else
								if( Irms>NO_LOAD_I && start_meter==ON ){
									/*tmp = ReadRtc_sec();
									if( energy_start != tmp ){
										energy_start = tmp;
										++energy_count;
									}*/
									if( chrono_timeout(energy_start, INTEGRATION_TIME) ){
										energy_start = chrono_start();
										ActivEnergy_W += ActiveEnergy_asic();
										asic_status = ENERGY;
									}
									
									/*if( energy_count == INTEGRATION_TIME){
										energy_count = 0;
										ActivEnergy_W += ActiveEnergy_asic();
										asic_status = ENERGY;
									}*/
								}
				}
			}
			else
				if( isr_code==ZXTO ){
					Urms = 0;
					Irms = 0;
					//disableIsr_asic( ZX );
					//disableIsr_asic( ZXTO );
					asic_status = COMPLETE;
					//break;
				}
				else
					asic_status = WAIT_ISR;
			break;
		case ENERGY:
			if( ActivEnergy_W>=UM_Wh ){
				ActivEnergy += ActivEnergy_W;//UM_Wh;
				//ActivEnergy_W -= UM_Wh;
				ActivEnergy_W = 0;
				watt_inc = ON;	
			}	
			asic_status = COMPLETE;
			break;
		case COMPLETE:
			break;							
	}
}				
					
					
					
					
/*					
					
					if( Irms<NO_LOAD_I && start_meter==ON ){
						ActivEnergy_W += ActiveEnergy_asic();
						start_meter = OFF;
					}
					if( Irms>=NO_LOAD_I ){
						
						ActivEnergy_W += ActiveEnergy_asic();
					
					
					if( ActivEnergy_W>=UM_Wh ){
						ActivEnergy += UM_Wh;
						ActivEnergy_W -= UM_Wh;
						watt_inc = ON;
						start_meter = ON;
					}
					//else
						//ActiveEnergy_asic(); // limpar
					//disableIsr_asic( ZX );
					//disableIsr_asic( ZXTO );
					asic_status = COMPLETE;	
					//break;
				}
			}
			else
				if( isr_code==ZXTO ){
					Urms = 0;
					Irms = 0;
					//disableIsr_asic( ZX );
					//disableIsr_asic( ZXTO );
					asic_status = COMPLETE;
					//break;
				}
				else 
					asic_status = WAIT_ISR;
			break;
		case COMPLETE:
			break;
	}
}
			*/		

			
		
		
		


// Calibrar energia activa
/*
unsigned int CaliberActiveEnergy_asic(){
	unsigned char t[3];
	unsigned int tmp, tmp_apparent, tmp_reactive;
	float x;

	// Set Mode
	t[0] = 0x09;
	read_asic(t, 2);
	
	t[1] |= (1<<CYCMODE);
	
	t[2] = t[1];
	t[1] = t[0];
	t[0] = 0x09;
	write_asic(t);
	
	// LYNECYC =20 [s]
	t[0] = 0x1C;
	t[1] = 0x07;
	t[2] = 0xD0;
	write_asic(t);
	
	// Interrupt
	enableIsr_asic( CYCEND );
	
	isrStatus_asic();
	
	while(1){
		if( irq_asic() )
			if( isrStatus_asic()==CYCEND ){ 
				t[0] = 0x04; //LAENERGY
				read_asic(t, 3);
				tmp = t[0];
				tmp <<= 8;
				tmp |= t[1];
				tmp <<= 8;
				tmp |= t[2];
				x = tmp;
				
				teste[var++] = tmp;
				if(var==10)
					var+=8;
				
				t[0] = 0x07; //LAENERGY
				read_asic(t, 3);
				tmp_apparent = t[0];
				tmp_apparent <<= 8;
				tmp_apparent |= t[1];
				tmp_apparent <<= 8;
				tmp_apparent |= t[2];
				
				//return x;
				t[0] = 0x08; //LAENERGY
				read_asic(t, 3);
				tmp_reactive = t[0];
				tmp_reactive <<= 8;
				tmp_reactive |= t[1];
				tmp_reactive <<= 8;
				tmp_reactive |= t[2];
			}
	}
	return 0;
}*/





