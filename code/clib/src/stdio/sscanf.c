#include <stdio.h>
#include <_stdio.h>

static int scin(void * str, int ch) {
	char * s = *(char **) str;

	if (ch == _WANT)
		if (*s == '\0')
			return EOF;
		else {
			*(char **)str = s + 1;
			return *s;
		}
	else if (ch >= 0)
		*(char **) str = s - 1;
	return ch;
}

int sscanf(const char * buf, const char * fmt, ...) {
	int ans;
	va_list ap;

	va_start(ap, fmt);
	ans = _scanf(scin, (void **) & buf, fmt, ap);
	va_end(ap);
	return ans;
}
