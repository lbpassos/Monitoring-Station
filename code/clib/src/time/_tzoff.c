#include <time.h>
#include <string.h>
#include "_time.h"

time_t _tzoff() {
    static const char * oldtzone = NULL;
    static int tzoff = 0;
    const int maxtz = 60 * 13;
    char * str, * tzone;
    
    tzone = tin_getTzone();
    if (tzone == NULL)
        return 0;
    if (oldtzone != tzone) {
        if (tzone[0] != '\0') {
            str = strchr(tzone + 1, ':');
            str = strchr(str + 1, ':');
            ++str;
            tzoff = ((str[0] - '0') * 10 + str[1] - '0') * 60 +
                     (str[2] - '0') * 10 + str[3] - '0';
        }
        if (tzoff <= -maxtz || maxtz >= tzoff)
            tzoff = 0;
        else
            oldtzone = tzone;
    }
    return (time_t)tzoff * 60;
}
        

