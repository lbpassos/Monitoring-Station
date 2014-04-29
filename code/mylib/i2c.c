#include "i2c.h"
#include "lpc2106.h"

#define SCL	2
#define	SDA	3

#define RUN		0x00
#define END		0X01

static unsigned char slave_address; // end do slave

static unsigned char *data_tx; // dados a transmitir
static unsigned int dim_data_tx; // dim dos dados a transmitir
static unsigned char *data_rx; // dados a receber
static unsigned int dim_data_rx;// dim dos dados a receber

static unsigned char i2c_state; // estado da comunicacao
static unsigned char i2c_condition;//modo de comunicacao: TX, RX e TRX

/* Inicializar I2C */
void init_i2c(void){
	uint32_t tmp = PINSEL0; // selecao dos portos para i2c
	tmp = tmp & ~(3 << (SDA<<1)) & ~(3 << (SCL<<1));
	PINSEL0 = tmp | (1 << (SCL<<1)) | (1 << (SDA<<1)); // Seleccionar I2C
	/* freq = fpclk/(I2SCLH+I2SCLL) */
	/*200kHz -> I2SCLH=36 e I2SCLL=36*/
	I2SCLH = 0x24;
	I2SCLL = 0x24;
	I2CONCLR = 0x6C; // clears flags
	I2CONSET = 0x40; // Enable I2C	
}

/* i2c_set
 * Define o end. do slave e o modo(TX, RX e TRX)
 */
void i2c_set(unsigned char slave_add, unsigned char mode){
	slave_address = slave_add<<1;
	i2c_condition = mode;
}

/* i2c_comm
 * Define os parametros da comunicacao:
 *               datos a transmitir
 *               dimensao dos dados a transmitir
 *               destino dos dados recebidos
 *               dimensao dos dados a receber
 */
void i2c_comm(unsigned char *ptr_datatx, int dimtx, unsigned char *ptr_datarx, int dimrx){
	
	data_tx = ptr_datatx;
	dim_data_tx = dimtx;
	data_rx = ptr_datarx;
	dim_data_rx = dimrx;
	I2CONSET = 0x60; // Envio do START
	i2c_state = RUN;
}

/* i2c_status 
 * Verifica o estado de um pedido
 * Retorna: 1 - Pedido completo
 *          0 - Em processamento
 */
int i2c_status(void){
	if( i2c_state == END ) return 1; //escrita/leitura completa
	else return 0;
}


	


/* Interrupts i2c*/
void i2c_isrHandle(void){
	int handle = I2STAT;
	switch(handle){
		case 0x08: // Envio do end. do slave e a accao (leiura ou escrita) depois de um START
				if( i2c_condition==TRANSMIT_MODE || i2c_condition==TRANSCEIVER_MODE )
					I2DAT = slave_address; // address + R/W=0
				else 
					I2DAT = slave_address | 0x01; // address + R/W=1
				i2c_state = RUN;
				I2CONCLR = 0x28; // clear STA e SI
				break;
		case 0x10: // Envio do end. do slave para leiura depois de um START repetido
				I2DAT = slave_address | 0x01; // address + R/W=1
				I2CONCLR = 0x28;
				break;				
		case 0x18: // Envio do primeiro byte.
				if( !dim_data_tx ){
					I2CONSET = 0x50; // STOP flag
					i2c_state = END;
				}
				else{
					I2DAT = *(data_tx++);
					--dim_data_tx;
					i2c_state = RUN;
				}
				I2CONCLR = 0x08; // clear SI
				break;
		case 0x28: // Envio dos restantes bytes
				if( dim_data_tx ){
					I2DAT = *(data_tx++);
					--dim_data_tx;
					i2c_state = RUN;
				}
				else{
					if( i2c_condition==TRANSMIT_MODE ){
						I2CONSET = 0x50; // STOP flag
						i2c_state = END;
					}
					else
						if( i2c_condition==TRANSCEIVER_MODE ){
							I2CONSET = 0x60; //Repetir o START
							i2c_state = RUN;
						}
				}
				I2CONCLR = 0x08; // clear SI		
				break;	
		case 0x40: // Recepcao do primeiro byte
				if( dim_data_rx==1 )
					I2CONSET = 0x40; // so existe uma palavra para receber
				else
					I2CONSET = 0x44; //colocar AA=1
				i2c_state = RUN;
				I2CONCLR = 0x08; // clear SI
				break;
		case 0x50: // Recepcao dos seguintes bytes
				*(data_rx++) = I2DAT;
				--dim_data_rx;
				if( dim_data_rx == 1 ){
					I2CONCLR = 0x04; //Clear AA
					I2CONSET = 0x40;
				}
				else
					I2CONSET = 0x44;
				i2c_state = RUN;
				I2CONCLR = 0x08; // clear SI
				break;
		case 0x58: // Terminar recepcao
				*data_rx = I2DAT;
				I2CONSET = 0x50; // STOP
				I2CONCLR = 0x08; // clear SI
				i2c_state = END;
				break;
				
	}

}
		




	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
