#include <time.h>
#include <_time.h>

/*
time_t mktime(struct tm * t) {
    time_t secs = _tmtosecs(t) + _tzoff();
    struct tm * tp = localtime(&secs);
    if (t->tm_isdst > 0 || (t->tm_isdst < 0 && tp->tm_isdst > 0)) {
        secs += 3600;
        *t = *localtime(&secs);
    }
    else
        *t = *tp;
    return secs;
}

*/

time_t mktime(struct tm * t) {
    time_t secs = _tmtosecs(t);
    _secstotm(t, secs);
    if (_isdst(t) > 0) {
        t->tm_isdst = 1;
        return secs - 3600;
    }
    else {
        t->tm_isdst = 0;
        return secs;
    }
}


