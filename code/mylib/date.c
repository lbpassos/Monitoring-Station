#include "date.h"


#define JAN		31
#define	FEV		28
#define MAR		31
#define	ABR		30
#define MAI		31
#define	JUN		30
#define JUL		31
#define	AGO		31
#define SET		30
#define	OUT		31
#define NOV		30
#define DEZ		31

#define ANO_ZERO	2000


static const unsigned char daysMonth[] = { JAN, FEV, MAR, ABR, MAI, JUN, JUL, AGO, SET, OUT, NOV, DEZ };

static unsigned int year = 2013;
static unsigned char month = 5;
static unsigned char day = 1;

unsigned char year_Date(){ return year-ANO_ZERO; }
unsigned char month_Date(){ return month; }
unsigned char day_Date(){ return day; }

//Ano bissexto
unsigned char leapYear( unsigned int y){
	return (y%4==0) && (y%100) || (y%400==0);
}

int inc_Year(unsigned char y){
	++y;
	if( y==87 ) //rtc so funciona ate 2099( Ano 0 = 2013)
		y = 13;
	year = y + ANO_ZERO;
	return y;
}

int inc_Month(unsigned char m){
	++m;
	if( m==13 )
		m=1;
	month = m;
	return m;
}

int inc_Day(unsigned char d){
	++d;
	if( month==FEV && leapYear(year) ){
		if( d>29 )
			d = 1;
	}
	else
		if( d>daysMonth[month-1] )
			d = 1;
	day = d;
	return d;
}

int inc_Hour(unsigned char h){
	if( ++h > 23 )
		h = 0;
	return h;
}

int inc_Min(unsigned char m){
	if( ++m > 59 )
		m = 0;
	return m;
}
	




