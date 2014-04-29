#include "../mylib/lpc2106.h"
#include "../mylib/i2c.h"
#include "../mylib/32k_memmory.h"
#include "../mylib/hih6130.h"
#include "../mylib/mem_manager.h"
#include "../mylib/manager_display.h"
#include "../mylib/event.h"
#include "../mylib/asic.h"
#include "../mylib/wiznet.h"
#include "../mylib/snmp.h"

#define UM_S			1000
#define TEM_HUM_START	5 // minutos
#define TRINTA_S		30000//30s com pclk=1ms
#define SESSENTA_S		60000


//void irq_handler();



char humi[2];
char temp[2];
float Irms, Urms, Activenergy, Pfactor;
unsigned char recBuf[128];




/*---------------------------------------------------------------------------
Iniciar a infraestrutura de atendimento de interrupções
*/
static void interrupt_init() {
	/* Inibir todas as interrupções */
	VIC->IntEnClear = 0xffffffff;
	/* Limpar eventuais pedidos de interrupção via software */
	VIC->SoftIntClear = 0xffffffff;

	/* Mapear a tabela de interrupções, residente no inicio da ROM,
	no endereço 0x0000000 */
	MEMMAP = 1;
	
	/* Associar todas as fontes de interrupção à entrada IRQ */
	VIC->IntSelect = 0;
	
	/* Colocar o endereço da rotina de serviço IRQ na tabela de exceções */
	//* (void (**)()) (4 * 8 + 4 * 6) = irq_handler;
	
	/* Permitir que hajam interrupções ao nível do processador */
	interrupt_enable();
	
}

/*---------------------------------------------------------------------------
Desmascarar um pedido de interrupção ao controlador de interrupções
*/
void interrupt_unmask(int irq) {
	VIC->IntEnable = 1 << irq;
}

/*---------------------------------------------------------------------------
Mascarar Interrupts
*/
void interrup_mask(int irq){
	VIC->IntEnClear = 1 << irq;
}

/*---------------------------------------------------------------------------
Atender Interrupts
*/
void isr() {
	unsigned long irq_status = VIC->IRQStatus;
	if (irq_status & (1 << VIC_SOURCE_I2C))
		i2c_isrHandle();
	/*else
		if(irq_status & (1 << VIC_SOURCE_RTC))
			rtc_isrHandle();*/
}



void registador(){

	
	//event_timer_start_THIRD( TEM_HUM_START );
	
	//while (1) {
		event_t event;
		event_read(&event);
		int *ptr;
		switch(event.type) {
			case EVENT_KEY_PRESSED: //Tecla premida
					lcd_backlight(1);
					event_timer_start_SEC(FIRST_SEC_TIMER, TRINTA_S);
					break;
			case EVENT_NOKIA_TIMEOUT:
					lcd_backlight(0);
					break;
			case EVENT_CHECK_INFRA:
					initMeasure_hih6130(); //Iniciar leitura				
					event_timer_start_SEC(SEC_SEC_TIMER, 0); // Verificar infra em 1 min
					break;
			case EVENT_READ_INFRA:
					hum_hih6130( humi );
					temp_hih6130( temp );
					erase_temperatura();
					erase_humidade();
					display_temperatura(temp[0], temp[1]);
					display_humidade(humi[0], humi[1]);
					endMeasure_hih6130();
					event_timer_start_SEC(SEC_SEC_TIMER, SESSENTA_S); // Verificar infra em 1 min
					break;
			case EVENT_CHECK_U_I:
					startRMS_measure_asic(); //Iniciar processo de aquisicao
					Irms = Irms_value();
					Urms = Urms_value();
					erase_voltage();
					display_voltage(Urms);
					erase_current();
					display_current(Irms);
					
					//endRMS_measure_asic();
					//event_timer_start_SEC(THIRD_SEC_TIMER, 1);
					break;
			case EVENT_CHECK_ENERGY:
					Activenergy = ActiveE_value();
					//Pfactor = PowerFactor();
					erase_consumo();
					display_consumo(Activenergy);
					//erase_pFactor();					
					//display_pFactor( Pfactor );
					//event_timer_start_SEC(FOURTH_SEC_TIMER, 60); //5 minutos
					break;
			case EVENT_SNMP:
					proc_agent_snmp();
					break;		
		}
	//}
}		
				
				
					
					


			
//int main()__attribute__((section(".text.fastcode")));		
int main() {
	unsigned int ip_address, mask_address;
	
	watchdog_init();
	counter_init();
	init_i2c();
	spi_init();
	//rtc_init(); // Iniciar RTC
	interrupt_init();
	interrupt_unmask(VIC_SOURCE_I2C); // I2C
	//interrupt_unmask(VIC_SOURCE_RTC); // RTC 
	//init_memmory();
	
	button_init();
	//init_managerButton();
	event_init();
	asic_init();
	/* WIZNET */
	reset_wiznet(); 
	wiznet_init();
	
	ip_address = 192;
	ip_address <<= 8;
	ip_address |= 168;
	ip_address <<= 8;
	ip_address |= 0;
	ip_address <<= 8;
	ip_address |= 1;
	
	mask_address = 255;
	mask_address <<= 8;
	mask_address |= 255;
	mask_address <<= 8;
	mask_address |= 255;
	mask_address <<= 8;
	mask_address |= 0;
	
	setIP_wiznet( ip_address ); //definir ip_address
	setMASK_wiznet(mask_address); //definir mask_address
	unmaskIRQ_wiznet(IM_IR0);
	init_agent_snmp(recBuf, recBuf, 128, 128);
	initVariables_snmp(&Urms, &Irms, &Activenergy, temp, humi);
	
	
	
	reset_asic();
	startRMS_measure_asic();
	enableIsr_asic( ZX );
	enableIsr_asic( ZXTO );
	/* LCD */
	lcd_reset();
	lcd_io_init();
	lcd_reset();
	lcd_io_init();
	
	display_erase();
	display_title();
	event_timer_start_SEC(SEC_SEC_TIMER, SESSENTA_S); //Infra: 1min
	//event_timer_start_SEC(THIRD_SEC_TIMER, 1); //U_I 1seg
	//event_timer_start_SEC(FOURTH_SEC_TIMER, 60); //Enegia 1 minutos

	
	displayMenu_voltage();
	displayMenu_corrente();
	//displayMenu_pFactor();
	displayMenu_consumo();
	displayMenu_temperatura();
	displayMenu_humidade();
	
	isrStatus_asic(); //Limpar reset irq no asic
	

	//CaliberActiveEnergy_asic();
	/*hum_hih6130( humi );
	temp_hih6130( temp);
	endMeasure_hih6130();
	display_temperatura(temp[0], temp[1]);
	display_humidade(humi[0], humi[1]);
	read_rtc(&rtc_clock);
	display_data( clockData_Read_year(&rtc_clock)<<4 | clockData_Read_month(&rtc_clock), clockData_Read_day(&rtc_clock), clockData_Read_hour(&rtc_clock), clockData_Read_min(&rtc_clock) );
	*/
	//lcd_backlight(1);
	//event_timer_start_SECOND( UM_S, SESSENTA_S );
	//event_counter_minutes( TEM_HUM_START );

	
	
	while(1){
		registador();	
		hih6130();
		asic_rms();
		watchdog_reload();
	}
	return 0;
}
