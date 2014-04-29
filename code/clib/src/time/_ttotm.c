#include <time.h>
#include <_time.h>

/*
 *  Converte um tempo expresso em segundos deste 00:00:00 1/1/1900
 *  para uma data expressa na struct tm
 *
 *  isdst > 0   Estamos no periodo de hora de verao e o ajuste ja foi feito
 *  isdst = 0   Nao estamos no periodo de hora de verao
 *  isdst < 0   O chamador nao sabe se estamos no periodo de hora de verao
 */
struct tm * _ttotm(struct tm * t, time_t secsarg, int isdst) {
    int year;
    time_t secs;
    static struct tm ts;

    if (t == NULL)
        t = &ts;
    t->tm_isdst = isdst;
    secsarg += _TBIAS;  
    for (secs = secsarg; ; secs = secsarg + 3600) {
        {
            long aux, days = secs / 86400;
            t->tm_wday = (days + WDAY) % 7;
            year = days / 365; 
            while (days < (aux = days1900(year, 0, 1)))
                --year;
            t->tm_year = year;
            t->tm_yday = days - aux;
        }
        {
            int mon, aux, days = t->tm_yday;
            mon = days / 31;
            while (days > (aux = yearDays(year, mon, 1)))
                ++mon;
            t->tm_mon = mon;
            t->tm_mday = days - aux + 1;
        }
        {
            secs %= 86400L;
            t->tm_hour = secs / 3600;
            secs %= 3600;
            t->tm_min = secs / 60;
            t->tm_sec = secs % 60;
        }
        if (t->tm_isdst >= 0 || (t->tm_isdst = _isdst(t)) <= 0)
            return t;
    }
}
