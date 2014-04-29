//
//  Para teste das funções do 'time'
//

#include <assert.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#include "_time.h"

#ifdef XXX

void main() {
     struct tm tx[4], tmp;
     int n;

     tmp.tm_sec = 00;
     tmp.tm_min = 00;
     tmp.tm_hour = 0;
     tmp.tm_mday = 1;
     tmp.tm_mon = 0;
     tmp.tm_year = 98;
     tmp.tm_wday = 0;
     tmp.tm_yday = 0;
     tmp.tm_isdst = -1;
     n = _getDstDate(&tmp, tx);
}


static const int _daysMonthL[] =
     {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};

static const int _daysMonth[] =
     {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

#define leap(y) ((y & 3) == 0)

#define yearDays(y, m, d) ((leap(y) ? _daysMonthL[m] : _daysMonth[m]) + d - 1)

#define days1900(y, m, d) ((long) y * 365 + (y - 1) / 4 + yearDays(y, m, d))

#define weekDay(y, m, d)  ((days1900(y, m, d) + WDAY ) % 7)


#endif


void main(void) {
    struct tm t, * tmp = &t;
    time_t tp;
    time_t secs, secs1, secs2, secs3, secs4;
    int i, j;

    tmp->tm_sec = 59;
    tmp->tm_min = 59;
    tmp->tm_hour = 1;
    tmp->tm_mday = 28;
    tmp->tm_mon = 2;
    tmp->tm_year = 99;
    tmp->tm_wday = 0;
    tmp->tm_yday = 0;

    for (;;) {
        int c;
        c = getch();
        if (c == 'a')
             --tmp->tm_mday;
        else if (c == 'q')
             ++tmp->tm_mday;

        else if (c == 's')
             --tmp->tm_mon;
        else if (c == 'w')
             ++tmp->tm_mon;

        else if (c == 'd')
             --tmp->tm_year;
        else if (c == 'e')
             ++tmp->tm_year;

        else if (c == 'f')
             --tmp->tm_hour;
        else if (c == 'r')
             ++tmp->tm_hour;

        else if (c == 'g')
             --tmp->tm_min;
        else if (c == 't')
             ++tmp->tm_min;

        else if (c == 'h')
             --tmp->tm_sec;
        else if (c == 'y')
             ++tmp->tm_sec;


        else if (c == 'z')
             tmp->tm_isdst = 1;
        else if (c == 'x')
             tmp->tm_isdst = 0;
        else if (c == 'c')
             tmp->tm_isdst = -1;
        else if (c == 'p')
             break;
        else if (c == 'm')
            secs = mktime(tmp); // + days1900(70, 0, 1) * 86400L - timezone;
  
        printf("[%d][%lu] %s", tmp->tm_isdst, secs, asctime(tmp));
    }
}

#ifdef  XXX

void main() {
     char buf[32];
     clock_t tc = clock();
     struct tm ts1;
     time_t tt1, tt2;
     static char *dstr = " Sun Dec 2 06:55:15 1979\n";

     tt1 = time(&tt2);
     assert(tt1 == tt2);
     ts1.tm_sec = 59;
     ts1.tm_min = 59;
     ts1.tm_hour = 1;
     ts1.tm_mday = 6;
     ts1.tm_mon = 3;
     ts1.tm_year = 97;
     ts1.tm_isdst = -1;
     tt1 = mktime(&ts1);
     assert(ts1.tm_wday == 0);
     assert(ts1.tm_yday == 335);
     ++ts1.tm_sec;
     tt2 = mktime(&ts1);
     assert(difftime(tt1, tt2) == 1);
     assert(strcmp(asctime(localtime(&tt1)), dstr) == 0);
     assert(strftime(buf, sizeof(buf), "%S", gmtime(&tt2)) == 2);
     assert(strcmp(buf, "16") == 0);
     assert(tc <= clock());
     time(&tt1);
}

#endif


