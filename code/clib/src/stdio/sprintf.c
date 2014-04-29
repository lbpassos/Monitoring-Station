#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <_stdio.h>

static void * prout(void * s, const char * buf, size_t n) {
    return (char *) memcpy(s, buf, n) + n;
}

int sprintf (char * s, const char * fmt, ...) {
    int n;
    va_list ap;

    va_start(ap, fmt);
    n = _printf(prout, s, INT_MAX, fmt, ap);
    if (n >= 0) 
        s[n] = '\0';
    va_end(ap);
    return n;
}

