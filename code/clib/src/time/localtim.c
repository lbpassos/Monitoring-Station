#include <time.h>
#include <_time.h>

struct tm * localtime(const time_t * time) {
    static struct tm t;
    time_t aux = *time - _tzoff();
    _secstotm(&t, aux);
    if (_isdst(&t) > 0) {
        _secstotm(&t, aux + 3600);
        t.tm_isdst = 1;
    }
    else 
        t.tm_isdst = 0;
    return &t;
}
