#include <stddef.h>

void * memset(void *s, int c, size_t size) {
 	char * p = s;
	while (size--)
		*p++ = c;
	return s;
}
