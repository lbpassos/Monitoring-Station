#include "lpc2106.h"

//static unsigned long periodNumber;

static const unsigned int MAX_TIMER_32bit = 0xFFFFFFFF;

void counter_init(){
	TIMER0_TCR = 2; //TCR reset
	TIMER0_PR = PCLK / 1000;// TC Fica com uma frequência de 1Khz.
	TIMER0_TCR	=	0x01;	//T0->TCR = LPC2XXX_TxTCR_CTR_ENABLE;

}

void watchdog_init(){
	WDTC = 210000000; //1 minuto
	WDMOD = 0x03; //Watchdog enable e Reset enable
	WDFEED = 0xAA;
	WDFEED = 0x55;
}

//void watchdog_reload()__attribute__((section(".text.fastcode")));
void watchdog_reload(){
	WDFEED = 0xAA;
	WDFEED = 0x55;
}
	
	
	
unsigned int counter_read(){
	return TIMER0_TC;
}
 
unsigned int chrono_elapsed(unsigned int initial){
	return counter_read() - initial;
}

void chrono_delay(unsigned int msec){
	unsigned int current = counter_read();
	while( chrono_elapsed(current)<msec);
}

unsigned int chrono_start(){ return  counter_read(); }

unsigned int chrono_timeout(unsigned int initial, unsigned int delta){
	unsigned int t = counter_read();
	
	if( (t-initial) >= delta ) return 1;
	else
		if( (t-initial)<0 ){
			t += (MAX_TIMER_32bit - initial) + 1;
			if( t >= delta )
				return 1;
		}
	return 0;
}


unsigned long int chrono_restart(unsigned long int initial, unsigned long int delta){
	return initial + delta;
}











	
	
