#include <stdlib.h>
#include <_stdlib.h>

long atol(const char * s) {
	return (long) _stoul(s, NULL, 10);
}
