/*
typedef struct _clockData{
	int sec;
	int min;
	int hour;
	int dayMonth;
	int month;
	int year;
} rtclock;

void clockData_Set_sec(rtclock *clk, int s);
void clockData_Set_min(rtclock *clk, int m);
void clockData_Set_hour(rtclock *clk, int h);
void clockData_Set_day(rtclock *clk, int d);
void clockData_Set_month(rtclock *clk, int m);
void clockData_Set_year(rtclock *clk, int y);

int clockData_Read_min(rtclock *clk);
int clockData_Read_hour(rtclock *clk);
int clockData_Read_day(rtclock *clk);
int clockData_Read_month(rtclock *clk);
int clockData_Read_year(rtclock *clk);
*/

void rtc_init();
char ReadRtc_sec();

/*
void set_rtc(rtclock *clk);
void read_rtc(rtclock *clk);
void rtc_isrHandle();
int rtc_MinuteStatus();
*/
