#include <time.h>
#include "_time.h"

char * _cTimeInfo[] = {
    ":AM:PM",
    ":Sun:Sunday:Mon:Monday:Tue:Tuesday:Wed:Wednesday:Thu:Thursday:Fri:Friday:Sat:Saturday",
    "|%b %D %H:%M:%S %Y|%b %D %Y|%H:%M:%S",
    ":040602:102602",
    ":Jan:January:Feb:February:Mar:March:Apr:April:May:May:Jun:June:Jul:July:Aug:August:Sep:September:Oct:October:Mov:November:Dec:December",
    ":EST:EDT:+0300"
};

char * _timeInfo[] = {
    "",
    ":Dom:Domingo:Seg:Segunda-Feira:Ter:Terça-Feira:Qua:Quarta-Feira:"
        "Qui:Quinta-Feira:Sex:Sexta-Feira:Sab:Sábado",
    "|%A, %D de %B de %Y as %H:%M:%S|%D %b %Y|%H:%M:%S",
    ":040602:102602",
    ":Jan:Janeiro:Fev:Fevereiro:Mar:Março:Abr:Abril:Mai:Maio:Jun:Junho:"
        "Jul:Julho:Ago:Agosto:Set:Setembro:Out:Outubro:Mov:Novembro:Dez:Dezembro",
    ""
};

/*
_TimeInfo * tin_getTinfo() {
    return &_timeInfo;
}
*/

char * tin_getAmPm(void) {
    return _cTimeInfo[0];
}

char * tin_getDays(void) {
    return _cTimeInfo[1];
}

char * tin_getFormats(void) {
    return _cTimeInfo[2];
}

char * tin_getDst(void) {
    return _cTimeInfo[3];
}

char * tin_getMonths(void) {
    return _cTimeInfo[4];
}

char * tin_getTzone(void) {
    return _cTimeInfo[5];
}
