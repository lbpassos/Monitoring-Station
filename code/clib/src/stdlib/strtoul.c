#include <stdlib.h>
#include <_stdlib.h>

unsigned long strtoul (const char * s, char * * endptr, int base) {
	return _stoul(s, endptr, base);
}
