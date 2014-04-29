#include <time.h>
#include "_time.h"

size_t strftime(char * s, size_t n, const char *fmt, const struct tm * t) {
	return _strftime(s, n, fmt, t);
}
