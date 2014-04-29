#include <string.h>

char * strncat(char * dst, const char * src, size_t size) {
	char * aux = dst;
	int i;
	while (*dst)
		dst++;
	for (i = 0; i < size && *src; i++)
		*dst++ = *src++;
	if (i == size)
		*dst = 0;
	return aux;
}
