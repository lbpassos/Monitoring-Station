#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <_stdlib.h>

long strtol (const char * s, char * * endptr, int base) {
	const char * sc;
	unsigned long x;

	for (sc = s; isspace(*sc); ++sc)
		;
	x = _stoul(s, endptr, base);
	if (*sc == '-' && x <= LONG_MAX) {
		errno = ERANGE;
		return LONG_MIN;
	}
	else if (*sc != '-' && x > LONG_MAX) {
		errno = ERANGE;
		return LONG_MAX;
	}
	else
		return (long) x;
}
