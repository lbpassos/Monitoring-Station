#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>

#define	BASE_MAX	16

static const char digits[] = {"0123456789abcdef"};

static const char ndigs[BASE_MAX + 1] =
	{0, 0, 33, 21, 17, 14, 13, 12, 11, 11, 10, 10, 9, 9, 9, 9, 9};

unsigned long _stoul(const char * s, char * * endptr, int base) {
	const char * sc, * sd;
	const char * s1, * s2;
	char sign;
	ptrdiff_t n;
	unsigned long x, y, u;

	for (sc = s; isspace(*sc); ++sc)
		;
	sign = *sc == '-' || *sc == '+' ? *sc++ : '+';
	if (base < 0 || base == 1 || base > BASE_MAX) {
		if (endptr)
			*endptr = (char *)s;
		return 0;
	}
	else if (base > 0) {
		if (base == 16 && sc[0] == '0' && (sc[1] == 'x' || sc[1] == 'X'))
			sc +=2;
	}
	else if (*sc != '0')
		base = 10;
	else if (sc[1] == 'x' || sc[1] == 'X')
		base = 16, sc += 2;
	else 
		base = 8;
	for (s1 = sc; *sc == '0'; ++sc)
		;
	x = 0;

	for (s2 = sc; (sd = memchr(digits, tolower(*sc), base)) != NULL; ++sc) {
		y = x;
		u = sd - digits;
		x = x * base + u;
	}

	if (s1 == sc) {
		if (endptr)
			*endptr = (char *)s;
		return 0;
	}
	n = sc - s2 - ndigs[base];
	if (n < 0)
		;
	else if (n > 0 || x < x - u || (x - u) / base != y ) {
		errno = ERANGE;
		x = ULONG_MAX;
	}
	
	if (sign == '-')
		x = -x;
	if (endptr)
		*endptr = (char *)sc;
	return x;
}
