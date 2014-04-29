#include <string.h>

char * strcat(char * dst, const char * src) {
	char * aux = dst;
	while (*dst)
		dst++;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return aux;
}
