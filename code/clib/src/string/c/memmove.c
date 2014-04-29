#include <stddef.h>

void *memmove( void *dest, const void *src, size_t count ) {
	char* d = dest, * s = src;
	if (d < s)
		while(count--)
			*d++ = *s++;
	else {
		d += count;
		s += count;
		while(count--)
			*--d = *--s;
	}
	return dest;
}
