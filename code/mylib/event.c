#include "event.h"
#include "lpc2106.h"
#include "wiznet.h"


#define	INIT						0
#define SEARCH_KEY_PRESSED			1
#define SEARCH_TIME_OUT				2
#define SEARCH_CHECK_INFRA			3
#define SEARCH_MEASURE_INFRAckeck	4
#define SEARCH_CHECK_U_I_rms		5
#define SEARCH_MEASURE_ENERGY		6
#define SEARCH_SNMP					7
#define RELOAD_WATCHDOG				8



static Milisecond_t table_msecond[MAX_SEC_TIMERS];
static unsigned char status, buttonWas_pressed;

void event_init(){
	status = SEARCH_KEY_PRESSED;
}
	
	
void event_read(event_t *ptr){
	
	ptr->type = 0;
	
	switch(status){
		case INIT:
			break;
		case SEARCH_KEY_PRESSED: //Tecla premida
			if( button_pressed() ){
				button_read();
				buttonWas_pressed = 1;
				ptr->type = EVENT_KEY_PRESSED; // botao premido
			}
			status = SEARCH_TIME_OUT;
			break;
		case SEARCH_TIME_OUT: //Tempo de BL=1
			if( buttonWas_pressed ){
				if( timeOut_event( FIRST_SEC_TIMER ) ){
					buttonWas_pressed = 0;
					ptr->type = EVENT_NOKIA_TIMEOUT;
				}
			}
			status = SEARCH_CHECK_INFRA;
			break;
		case SEARCH_CHECK_INFRA: //Verificar TEMP e HUM
			if( timeOut_event( SEC_SEC_TIMER ) )
				ptr->type = EVENT_CHECK_INFRA;
			status = SEARCH_MEASURE_INFRAckeck;
			break;
		case SEARCH_MEASURE_INFRAckeck:	
			if( status_hih30() )
				ptr->type = EVENT_READ_INFRA;
			status = SEARCH_CHECK_U_I_rms;
			break;
		case SEARCH_CHECK_U_I_rms:
			if( measureRMS_complete_asic() )
				ptr->type = EVENT_CHECK_U_I;
			status = SEARCH_MEASURE_ENERGY;
			break;
		case SEARCH_MEASURE_ENERGY:
			if( isWattInc() )
				ptr->type = EVENT_CHECK_ENERGY;
			status = SEARCH_SNMP;
			break;
		case SEARCH_SNMP:
			if( IRQ_wiznet() )
				if( causeIRQ_wiznet()==IM_IR0 )
					ptr->type = EVENT_SNMP;
			status = SEARCH_KEY_PRESSED;
			break;
		/*case RELOAD_WATCHDOG:
			watchdog_reload();
			status = SEARCH_KEY_PRESSED;
			break;*/
	}
}	

void event_timer_start_SEC(unsigned int timer_def, unsigned int s){
	table_msecond[timer_def].delta = s;
	table_msecond[timer_def].initial = chrono_start(); // valor actual
}

int timeOut_event(unsigned char timer_def){
	if( table_msecond[timer_def].delta==0 )
		return 0;
	if( chrono_timeout( table_msecond[timer_def].initial, table_msecond[timer_def].delta) )
		return 1;
	return 0;
}
	
/*		
void event_timer_start_SEC(unsigned char timer_def, unsigned int s){
	table_second[timer_def].chrono = s;
	table_second[timer_def].initial = ReadRtc_sec(); // valor actual
}

int timeOut_second(unsigned char timer_def){
	unsigned char tmp;
	
	if( table_second[timer_def].chrono==0 )
		return 0;
		
	tmp = ReadRtc_sec();
	if( tmp != table_second[timer_def].initial ){
		table_second[timer_def].initial = tmp;
		--table_second[timer_def].chrono;
		if( !table_second[timer_def].chrono )
			return 1;
	}
	return 0;
}*/
		
