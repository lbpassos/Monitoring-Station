#include "rtc.h"
#include "lpc2106.h"

/*static unsigned char Minute_int;

void clockData_Set_sec(rtclock *clk, int s){ clk->sec = s; }
void clockData_Set_min(rtclock *clk, int m){	clk->min = m; }
void clockData_Set_hour(rtclock *clk, int h){ 	clk->hour = h; }
void clockData_Set_day(rtclock *clk, int d){ 	clk->dayMonth = d; }
void clockData_Set_month(rtclock *clk, int m){	clk->month = m; }
void clockData_Set_year(rtclock *clk, int y){	clk->year = y; }

int clockData_Read_min(rtclock *clk){ 	return clk->min; }
int clockData_Read_hour(rtclock *clk){	return clk->hour; }
int clockData_Read_day(rtclock *clk){	return clk->dayMonth; }
int clockData_Read_month(rtclock *clk){	return clk->month;	}
int clockData_Read_year(rtclock *clk){	return clk->year; }
*/

char ReadRtc_sec(){ //valor do segundo
	return SEC;
}

void rtc_init(){
	SEC = 0; // por segundos a 0
	MIN = 0;
	//HOUR = 0;
	//YEAR = 2013;
	//DOM = 23;
	//MONTH = 5;
	/* pclk configurado na PLL a 14MHz */
	PREINT = PCLK/32768 - 1;
	PREFRAC = PCLK - ((PREINT+1) * 32768);
	//ILR = 0x03; // Clear interrupts
	//CIIR = 2; // Activar IMMIN - interrupt ao minuto
	AMR = 0xFF; // Disable interrupts registo
	CCR = 0x02; //CTRST=1     e    CLKEN=0
	CCR = 0x01; //CTRST=0     e    CLKEN=1
}

/*
void set_rtc(rtclock *clk){
	CCR = 0x02; //CTRST=1     e    CLKEN=0
	SEC = clk->sec;
	MIN	= clk->min;
	HOUR = clk->hour;
	DOM = clk->dayMonth;
	MONTH = clk->month;
	YEAR = clk->year;
	CCR = 0x01; //CTRST=0     e    CLKEN=1
}

void read_rtc(rtclock *clk){
	int aux0, aux_a0, aux1, aux_a1;
	
	aux0 = CTIME_0; // min, hora
	aux1 = CTIME_1; // dia, mes e ano
	
	do{
		aux_a0 = aux0;
		aux_a1 = aux1;
		aux0 = CTIME_0;
		aux1 = CTIME_1;
	}
	while( aux_a0!=aux0 || aux_a1!=aux1 ); // Garantir que a leitura da data e a correcta
	
	clk->min = MIN;
	clk->hour = HOUR;
	clk->dayMonth = DOM;
	clk->month = MONTH;
	clk->year = YEAR;
}

void rtc_isrHandle(){
	Minute_int = 1;
	ILR = 1; //Limpar interrupt
}

int rtc_MinuteStatus(){
	//ILR = 1; //Limpar interrupt
	if( Minute_int ){
		Minute_int = 0;
		return 1;
	}
	return 0;
}
	
*/
	
	
