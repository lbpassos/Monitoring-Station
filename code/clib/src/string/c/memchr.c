#include <stddef.h>

void * memchr(void *s, int c, size_t size) {
 	char * p = s;
	for (; size--; ++p)
		if (*p == c)
			return p;
	return NULL;
}
