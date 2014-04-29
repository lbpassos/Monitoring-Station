#include <stdlib.h>

static int toa(long value, char * str, int radix) {
	ldiv_t aux;
	int n;
	if (value == 0)
		return 0;
	aux = ldiv(value, radix);
	n = toa(aux.quot, str, radix);
	str[n] = aux.rem > 9 ? aux.rem - 10 + 'A' : aux.rem + '0';
	return n + 1;
}	

void _longtoa(long value, char * str, int radix, int signal) {
	if (value == 0) {
		str[0] = '0';
		str[1] = '\0';
	}
	if (signal && value < 0) {
		*str++ = '-';
		value = -value;
	}
	str[toa(value, str, radix)] = '\0';
}	
	
