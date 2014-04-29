#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <_stdio.h>
#include <_global.h>

typedef struct {
    unsigned int flags;
    int width, prec;
    char qual;
    char * s0;
    int n0, nz0;
    char * s1;
    int n1, nz1;
    char * s2;
    int n2, nz2;
    char buf[32];
    size_t nchar;
} _Pft;
                                            
static char udigs[] = "0123456789ABCDEF";
static char ldigs[] = "0123456789abcdef";

static int _itoa(long lval, char * str, size_t size, char code) {
    int i = size;
    char *digs = code == 'X' ? udigs : ldigs;
    int base = code == 'o' ? 8 : code == 'x' || code == 'X' ? 16 : 10;
    unsigned long ulval;
    
    ulval = ((code == 'd' || code == 'i') && lval < 0) ? -lval : lval; 
    do {
        str[--i] = digs[ulval % base];
    } while ((ulval /= base) > 0 && i > 0);
    return i;
}

static void _putfld(_Pft * px, va_list * pap, char code) {
    long lval;
    int i;

    px->n0 = px->nz0 = 0;
    px->n1 = px->nz1 = 0;
    px->n2 = px->nz2 = 0;
    px->s0 = px->s1 = px->s2 = px->buf;
    switch (code) {
        case 'c':
            px->buf[0] = va_arg(*pap, int);
            px->n0 = 1;
            break;
        case 'd': case 'i':
            lval = px->qual == 'l' ? va_arg(*pap, long) : va_arg(*pap, int);
            if (px->qual == 'h')
                lval = (short) lval;

            /* prefixo */
            if (lval < 0) {
                px->s0 = "-";
                px->n0 = 1;
            }
            else if (px->flags & _FPL) {
                px->s0 = "+";
                px->n0 = 1;
            }
            else if (px->flags & _FSP) {
                px->s0 = " ";
                px->n0 = 1;
            }

            /* valor */
            i = sizeof(px->buf);
            if (lval != 0 || px->prec != 0)
                i = _itoa(lval, px->buf, i, code);
            px->s1 = &px->buf[i];
            px->n1 = sizeof(px->buf) - i;

            /* zeros */
            if (px->n1 < px->prec)
                px->nz0 = px->prec - px->n1;
            else if ( px->prec < 0 && (px->flags & (_FMI | _FZE)) == _FZE)
                px->nz0 = px->width - px->n1 - px->n0;
            else
                px->nz0 = 0;
            break;
        case 'o': case 'u': 
        case 'x': case 'X': 
            lval = px->qual == 'l' ? va_arg(*pap, long) : va_arg(*pap, int);
            if (px->qual == 'h')
                lval = (unsigned short) lval;
            else if (px->qual == '\0')
                lval = (unsigned int) lval;

            if (px->flags & _FNO && lval != 0) {
                switch(code) {
                case 'o':
                    px->s0 = "0";
                    px->n0 = 1;
                    break;
                case 'x':
                    px->s0 = "0x";
                    px->n0 = 2;
                    break;
                case 'X':
                    px->s0 = "0X";
                    px->n0 = 2;
                }
            }

            /* valor */
            i = sizeof(px->buf);
            if (lval != 0 || px->prec != 0)
                i = _itoa(lval, px->buf, i, code);
            px->s1 = &px->buf[i];
            px->n1 = sizeof(px->buf) - i;

            /* zeros */
            if (px->n1 < px->prec)
                px->nz0 = px->prec - px->n1;
            else if ( px->prec < 0 && (px->flags & (_FMI | _FZE)) == _FZE)
                px->nz0 = px->width - px->n1 - px->n0;
            else
                px->nz0 = 0;
            break;
        case 'e': case 'E': case 'f':
        case 'g': case 'G':
            strcpy(px->buf, "<float>");
            px->n0 = strlen("<float>");
            break;
        case 'n':
            if (px->qual == 'h')
                *va_arg(*pap, short *) = px->nchar;
            else if (px->qual == 'l')
                *va_arg(*pap, long *) = px->nchar;
            else 
                *va_arg(*pap, int *) = px->nchar;
            break;
        case 'p':
            lval = (long)va_arg(*pap, void *);
            /* valor */
            i = sizeof(px->buf);
            if (lval != 0 || px->prec != 0)
                i = _itoa(lval, px->buf, i, 'x');
            px->s1 = &px->buf[i];
            px->n1 = sizeof(px->buf) - i;

            /* zeros */
            if (px->n1 < px->prec)
                px->nz0 = px->prec - px->n1;
            else if ( px->prec < 0 && (px->flags & (_FMI | _FZE)) == _FZE)
                px->nz0 = px->width - px->n1 - px->n0;
            else
                px->nz0 = 0;
            break;
        case 's':
            px->s0 = va_arg(*pap, char *);
            px->n0 = strlen(px->s0);
            if (px->prec >= 0 && px->prec < px->n0)
                px->n0 = px->prec;
            break;
        case '%':
            px->buf[0] = '%';
            px->n0 = 1;
            break;
        default:
            px->buf[0] = code;
            px->n0 = 1;
    }
}

#define PAD(b, n)                           \
    if ((n) > 0) {                          \
        int i, j = (n);                     \
        for (; j > 0; j -= i) {             \
            i = min(j, size(b) - 1);        \
            PUT(b, i);                      \
        }                                   \
    } else 

#define PUT(b, n)                                       \
    if ((n) > 0) {                                      \
        int len = min(count, (n));                      \
        if ((arg = ( * pfn)(arg, b, len)) != NULL) {    \
            x.nchar += len;                             \
            count -= len;                               \
            if (count == 0)                             \
                return EOF;                             \
        }                                               \
        else                                            \
            return EOF;                                 \
    }                                                   \
    else

static char spaces[] = "                                              ";
static char zeroes[] = "0000000000000000000000000000000000000000000000";

int _printf(void * ( * pfn) (void *, const char *, size_t),
    void * arg, size_t count, const char * fmt, va_list ap) {

    _Pft    x;

    x.nchar = 0;
    for ( ; ; ) {
        const char * s;
        {
            for (s = fmt; *s != '%' && *s != '\0'; s++)
                ;
            PUT(fmt, s - fmt);
            if (*s == '\0')
                return (x.nchar);
        }
        ++s;
        /* ------------------------------------------------------------- 
         * Processar os campos
         *  <flags>         <with>      <precision>     <qualifier>
         *      
         *      +                                           L
         *      -           numero      . numero            l
         *    espaco          *         .   *               h
         *      #
         *      0
         */
        {
            const char * t;
            static const char fchar[] = {" +-#0"};
            static const unsigned int fbit[]={_FSP, _FPL, _FMI, _FNO, _FZE, 0};

            /* flags */
            for (x.flags = 0; (t = strchr(fchar, *s)) != NULL; ++s)
                x.flags |= fbit[t - fchar];

            /* width */
            if (*s == '*') {
                x.width = va_arg(ap, int);
                if (x.width < 0) {
                    x.width = -x.width;
                    x.flags |= _FMI;
                }
                ++s;
            } else
                for (x.width = 0; isdigit(*s); ++s)
                    if (x.width < _WMAX)
                        x.width = x.width * 10 + *s - '0';

            /* precision */
            if (*s != '.')
                x.prec = -1;
            else if (*++s == '*') {
                x.prec = va_arg(ap, int);
                ++s;
            }
            else
                for (x.prec = 0; isdigit(*s); ++s)
                    if (x.prec < _WMAX)
                        x.prec = x.prec * 10 + *s - '0';
            /* qualifier */
            x.qual = strchr("hlL", *s) ? *s++ : '\0';
        }
            /* ------------------------------------------------------------- */
        {
            _putfld(&x, &ap, *s);
            x.width -= x.n0 + x.nz0 + x.n1 + x.nz1 + x.n2 + x.nz2;

            if (!(x.flags & _FMI)) {              
                PAD(spaces, x.width);   /* ajuste a direita */
			}
            PUT(x.s0, x.n0);
            PAD(zeroes, x.nz0);
            PUT(x.s1, x.n1);
            PAD(zeroes, x.nz1);
            PUT(x.s2, x.n2);
            PAD(zeroes, x.nz2);

            if (x.flags & _FMI) {
                PAD(spaces, x.width);   /* ajuste a direita */
            }
        }
        fmt = s + 1;
    }
}
