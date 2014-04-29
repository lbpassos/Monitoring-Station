#ifndef _STDLIB_H
#define _STDLIB_H

#if !defined(__CONFIG_H)
#include <_config.h>
#endif

#define NULL    _NULL

#ifndef _SIZE_T
#define _SIZE_T
typedef _size_t size_t;
#endif

#ifndef __cplusplus
#ifndef _WCHAR_T
#define _WCHAR_T
typedef _wchar_t    wchar_t;
#endif
#endif

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long    quot;
    long    rem;
} ldiv_t;

/* Maximum value returned by "rand" function
*/
#define RAND_MAX 0x7FFFU

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define MB_CUR_MAX 1

#ifdef __cplusplus
extern "C" {
#endif

void    abort(void);
int     abs(int x);
int     atexit(void ( *func)(void));
double  atof(const char  *s);
int     atoi(const char  *s);
long    atol(const char  *s);
void  * bsearch(const void * key, const void * base, size_t nelem, size_t width,
                int ( * fcmp)(const void *, const void *));
void  * calloc(size_t nitems, size_t size);
div_t   div(int numer, int denom);
void    exit(int status);
void    free(void  *block);
char  * getenv(const char  *name);
long    labs(long x);
ldiv_t  ldiv(long numer, long denom);
void  * malloc(size_t size);
int     mblen(const char  *s, size_t n);
size_t  mbstowcs(wchar_t  *pwcs, const char  *s, size_t n);
int     mbtowc(wchar_t  *pwc, const char  *s, size_t n);
void    qsort(void  *base, size_t nelem, size_t width,
        int  (* fcmp)(const void  *, const void  *));
int     rand(void);
void  * realloc(void  *block, size_t size);
void    srand(unsigned seed);
double  strtod(const char  *s, char  * *endptr);
long    strtol(const char  *s, char  * *endptr, int radix);
long double  _strtold(const char  *s, char  * *endptr);
unsigned long  strtoul(const char  *s, char  * *endptr, int radix);
int     system(const char  *command);
size_t  wcstombs(char  *s, const wchar_t  *pwcs, size_t n);
int     wctomb(char  *s, wchar_t wc);

/* Funcoes nao normalizadas */
char * itoa(int value, char * str, int radix);
char * ultoa(unsigned long value, char * str, int radix);
char * ltoa(long value, char * str, int radix);

#ifdef __cplusplus
}
#endif

#endif  /* _STDLIB_H */
