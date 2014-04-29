#include <string.h>

char * strncpy(char * dst, const char * src, size_t size) {
	char * aux = dst;
	int i;
	for (i = 0; i < size && *src; i++)
		*dst++ = *src++;
	for (     ; i < size; i++)
		*dst++ = 0;
	return aux;
}
