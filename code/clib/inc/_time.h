#ifndef TIME_H
#define TIME_H

//#define WDAY    1     //  1 de Janeiro de 1900
#define WDAY      4     //  1 de Janeiro de 1970

extern const int _daysMonth[];

extern const int _daysMonthL[];

#define _leap(y) ( (((y) + 1900) % 400) == 0 || ( (((y) + 1900) & 3) == 0 && (((y) + 1900) % 100) != 0 ) )

#define yearDays(y, m, d) ((_leap(y) ? _daysMonthL[m] : _daysMonth[m]) + (d) - 1)

#define _days1900(y, m, d) ((long) (y) * 365 + ((y) - 1) / 4 + yearDays(y, m, d))

#define days1970(y, m, d) (_days1900(y, m, d) - _days1900(70, 0, 1))

#define weekDay(y, m, d)  ((days1970(y, m, d) + WDAY ) % 7)

#ifdef __cplusplus
extern "C" {
#endif
	
void _secstotm(struct tm * t, time_t secs);

time_t _tmtosecs(struct tm * tm);

int _dayMonth(int, int);

time_t _tzoff(void);

size_t _strftime(char * buf, size_t bufsize, const char * fmt,
                    const struct tm * t);

int _isdst(const struct tm * t);

int _getDstDate(const struct tm * t, struct tm * tx);

char * tin_getAmPm(void);
char * tin_getDays(void);
char * tin_getFormats(void);
char * tin_getDst(void);
char * tin_getTzone(void);
char * tin_getMonths(void);

#ifdef __cplusplus
}
#endif

#endif
