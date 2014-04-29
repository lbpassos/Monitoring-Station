#include <stddef.h>

int memcmp( const void* buf1, const void* buf2, size_t count ) {
	const char* p1 = buf1, * p2 = buf2;
	for( ; count; count--, p1++, p2++)
		if (*p1 != *p2)
			return *p1 - *p2;
	return 0;
}	
