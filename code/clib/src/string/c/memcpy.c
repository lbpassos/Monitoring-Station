#include <stddef.h>

void* memcpy( void * dest, const void * src, size_t count ) {
	char * d = dest;
	const char * s = src;
	while(count--)
		*d++ = *s++;
	return dest;
}
