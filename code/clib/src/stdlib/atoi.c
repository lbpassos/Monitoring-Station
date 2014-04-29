#include <stdlib.h>
#include <_stdlib.h>

int atoi(const char * s) {
	return (int) _stoul(s, NULL, 10);
}
