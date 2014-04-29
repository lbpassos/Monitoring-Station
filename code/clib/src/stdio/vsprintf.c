#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <_stdio.h>

static void * prout(void * s, const char * buf, size_t n) {
	return (char *) memcpy(s, buf, n) + n;
}

int vsprintf (char * s, const char * fmt, va_list ap) {
	int n = _printf(prout, s, INT_MAX, fmt, ap);
	if (n >= 0)	
		s[n] = '\0';
	return n;
}
