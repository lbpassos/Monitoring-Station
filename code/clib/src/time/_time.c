#include <time.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "_time.h"

const int _daysMonthL[] =
    {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
const int _daysMonth[] =
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

/*
 *  Converte um valor expresso em segundos para uma struct tm
 */
void _secstotm(struct tm * t, time_t seconds) {
    int year;
    {
        long aux, days = seconds / 86400L;
        t->tm_wday = (days + WDAY) % 7;
        year = days / 365 + 70; 
        while (days < (aux = days1970(year, 0, 1)))
            --year;
        t->tm_year = year;
        t->tm_yday = days - aux;
    }
    {
        int mon, aux, days = t->tm_yday;
        mon = days / 29;
        while (days < (aux = yearDays(year, mon, 1)))
            --mon;
        t->tm_mon = mon;
        t->tm_mday = days - aux + 1;
    }
    {
        seconds %= 86400L;              //  segundos do dia
        t->tm_hour = seconds / 3600L;
        seconds %= 3600L;               // segundos da hora
        t->tm_min = seconds / 60;
        t->tm_sec = seconds % 60;
    }
}

/*
 *  Converte uma struct tm para um valor expresso em segundos
 */
time_t _tmtosecs(struct tm * t) {
    time_t secs;
    int ymon = t->tm_mon / 12;
    if (t->tm_year > INT_MAX - ymon)
        return -1;
    secs = 86400L * days1970(t->tm_year + ymon, t->tm_mon - ymon * 12, t->tm_mday);
    return secs += t->tm_hour * 3600L + t->tm_min * 60L + t->tm_sec;
}

/*
 *  Calcula o dia do mês
 */
int _dayMonth(int year, int yday) {
    int days, mon = yday / 30;
    while (yday < (days = yearDays(year, mon, 1)))
        --mon;
    return yday - days + 1;
}

