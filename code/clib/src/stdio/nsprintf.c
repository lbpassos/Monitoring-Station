#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <_stdio.h>

static void * prout(void * s, const char * buf, size_t n) {
    return (char *) memcpy(s, buf, n) + n;
}

int nsprintf (char * s, size_t size, const char * fmt, ...) {
    int n;
    va_list ap;

    va_start(ap, fmt);
    n = _printf(prout, s, size - 1, fmt, ap);
    if (n < 0 && size <= n) 
        n = size - 1;    
    s[n] = '\0';
    va_end(ap);
    return n;
}

