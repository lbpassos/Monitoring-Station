#include <ctype.h>
#include <string.h>
#include <time.h>
#include "_time.h"

/*
    Hora solar      Hora de verao       Relogio
            |
        22  +           23
            |
        23  +           00
            |
        00  +           01              00:59:58
            |                           00:59:59
        01  + - - - +   02              02:00:00
                    |                   02:00:01
        02          +   03
                    |
        03          +   04
                    |
        04          +   05

            . . .

        22          +   23
                    |
        23          +   00
                    |
        00          +   01              01:59:58
                    |                   01:59:59
        01  + - - - +   02              01:00:00
            |                           01:00:01
        02  +           03
            |
        03  +           04
            |
        04  +           05
            |   
*/

#define lenghtof(x) (sizeof(x)/sizeof(x[0]))

typedef struct {
    unsigned adap   : 1;    /* Regra fixa ou adaptativa */
    unsigned prev   : 1;    /* Para indicar um dia passado */
    unsigned wday   : 3;    /* Dia da semana */
    unsigned hour   : 5;    /* Hora */
    unsigned day    : 5;    /* Dia */
    unsigned mon    : 4;    /* Mes */
    unsigned year   : 7;    /* Ano */
} _DstRules;

static _DstRules _dstRules[10];

static int getint(const char * s, int n) {
    int value;
    for (value = 0; 0 <= --n && isdigit(*s); ++s)
        value = value * 10 + *s - '0';
    return 0 <= n ? -1 :value;
}

_DstRules * _getDst(const char * s) {
    const char delim = *s++;
    _DstRules * pr, * rules;

    if (delim == '\0')
        return NULL;
    {
        const char *s1, *s2;
        int i;

        for (s1 = s, i = 2; (s2 = strchr(s1, delim)) != NULL; s1 = s2 + 1, ++i)
            ;
        if (i > lenghtof(_dstRules))
            return NULL;
    }
    {
        int year = 0;

        for (pr = _dstRules; ; ++pr, ++s) {
            if (*s == '(') {
                year = getint(s + 1, 4) - 1900;
                if (year < 0 || s[5] != ')')
                    return NULL;
                s += 6;
            }
            pr->year = year;
            pr->mon = getint(s, 2) - 1, s += 2;
            pr->day = getint(s, 2), s += 2;
            if (isdigit(*s))
                pr->hour = getint(s, 2), s += 2;
            else
                pr->hour = 0;
            if (12 <= pr->mon || 99 < pr->day || 99 < pr->hour)
                return NULL;
            if (*s != '+' && *s != '-')
                pr->adap = 0;
            else if (s[1] < '0' || '6' < s[1])
                return NULL;
            else {
                pr->adap = 1;
                pr->wday = s[1] - '0';
                if (*s == '-')
                    pr->prev = 1;
                s += 2;
            }
            if (*s == '\0') {
                (pr + 1)->wday = 7;
                (pr + 1)->year = year;
                return _dstRules;
            }
        }
    }
}           

int _isdst(const struct tm * t) {

    _DstRules * pr;
    static _DstRules * rules = NULL;
    static const char * olddst = NULL;
    char * isdst = tin_getDst();

    if (isdst == NULL)
        return -1;
        
//    if (olddst != isdst) {
        if (isdst[0] != '\0') {
            if ((rules = _getDst(isdst)) == NULL)
                return -1;
        }
        else
            return -1;
        olddst = isdst;
//    }
    {
        int dst = 0;
        const int hour = t->tm_yday * 24 + t->tm_hour;
        for (pr = rules; pr->wday != 7; ++pr)
            if (pr->year <= t->tm_year) {
                /*
                 *  Dias desde o inicio do ano
                 */ 
                int yday = yearDays(t->tm_year, pr->mon, pr->day);

                /*
                 *  Testa se a regra de mudanca de hora e' fixa ou adaptativa
                 */     
		        if (pr->adap) {
		            if ( ! pr->prev) {	/* Definição a partir do inicio do mês */
			            int wday = weekDay(t->tm_year, pr->mon, 1);
				        yday = yearDays(t->tm_year, pr->mon, 1);
		    	        yday += pr->wday >= wday ? pr->wday - wday : pr->wday + 7 - wday;
						yday += (pr->day - 1) * 7;
		        	}
		        	else {	/* Definição a partir do fim do mês */
			            int wday = weekDay(t->tm_year, pr->mon + 1, 1);
				        yday = yearDays(t->tm_year, pr->mon + 1, 1);
		    	        yday -= wday > pr->wday ? wday - pr->wday : wday + 7 - pr->wday;
						yday -= (pr->day - 1) * 7;
		        	}
		        }   
                /*
                 *  Se a hora presente for menor que a proxima mudanca de hora
                 *  acabar a pesquisa
                 */
                if (hour < yday * 24 + pr->hour)
                    return dst;
                /*  
                 *  O inicio do ano comeca sempre em hora solar
                 *  Enquanto se mantiver no mesmo ano vai invertendo
                 */
                dst = pr->year == (pr + 1)->year ? ! dst : 0;
            }
        return dst;
    }
}

/*-----------------------------------------------------------------------------
 *  Obtenção de uma lista com as datas de mudança de hora
 */
int _getDstDate(const struct tm * t, struct tm * tx) {

    _DstRules * rules, * pr;
    char * isdst = tin_getDst();
    int i;
     
    if (isdst == NULL)
        return -1;
     
    /*
     *  Tradução das regras de formato texto para formato interno
     */
    
    if (isdst[0] != '\0') {
        if ((rules = _getDst(isdst)) == NULL)
            return 0;
    }
    else
        return 0;

    /*
     *  Interpretação das regras
     */
    for (i = 0, pr = rules; pr->wday != 7 && tx != NULL; ++pr, ++tx, ++i) {
        /*
         *  O ano em causa
         */ 
        int year = pr->year == 0 ? t->tm_year : pr->year;
        /*
         *  Dias desde o inicio do ano
         */ 
        int yday = yearDays(year, pr->mon, pr->day);

        /*
         *  Testa se a regra de mudanca de hora e' fixa ou adaptativa
         */     
        if (pr->adap) {
            if ( ! pr->prev) {	/* Definição a partir do inicio do mês */
	            int wday = weekDay(t->tm_year, pr->mon, 1);
		        yday = yearDays(year, pr->mon, 1);
    	        yday += pr->wday >= wday ? pr->wday - wday : pr->wday + 7 - wday;
				yday += (pr->day - 1) * 7;
        	}
        	else {	/* Definição a partir do fim do mês */
	            int wday = weekDay(t->tm_year, pr->mon + 1, 1);
		        yday = yearDays(year, pr->mon + 1, 1);
    	        yday -= wday > pr->wday ? wday - pr->wday : wday + 7 - pr->wday;
				yday -= (pr->day - 1) * 7;
        	}
        }   
       
        tx->tm_year = year;
        tx->tm_mon = pr->mon;
        tx->tm_mday = _dayMonth(year, yday);  //  dia no mês
        tx->tm_wday = (days1970(year, 0, 1) + yday + WDAY) % 7;  //  dia da semana
        tx->tm_yday = yday;
        tx->tm_hour = pr->hour;
        tx->tm_min = 0;
        tx->tm_sec = 0;
    }
    return i;                                                
}


