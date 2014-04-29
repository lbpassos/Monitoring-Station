#ifndef _TIME_H
#define _TIME_H

#ifndef __CONFIG_H
#include <_config.h>
#endif

#define	NULL	_NULL

#ifndef _SIZE_T
#define _SIZE_T
typedef _size_t size_t;
#endif

#define	CLOCKS_PER_SEC	_CPS

typedef unsigned int clock_t;
typedef unsigned long time_t;

struct tm {
  int   tm_sec;
  int   tm_min;
  int   tm_hour;
  int   tm_mday;
  int   tm_mon;
  int   tm_year;
  int   tm_wday;
  int   tm_yday;
  int   tm_isdst;
};

#ifdef __cplusplus
extern "C" {
#endif

char *		asctime(const struct tm  *tblock);
clock_t		clock(void);
char *		ctime(const time_t  *time);
time_t		difftime(time_t time2, time_t time1);
struct tm *	gmtime(const time_t  *timer);
struct tm *	localtime(const time_t  *timer);
time_t		mktime(struct tm  *timeptr);
size_t		strftime(char  *s, size_t maxsize, const char  *fmt, const struct tm  *t);
time_t		time(time_t  *timer);

#ifdef __cplusplus
}
#endif

#endif
