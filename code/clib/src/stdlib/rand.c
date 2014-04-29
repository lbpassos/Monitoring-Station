#include <stdlib.h>

extern unsigned long _randseed;

int rand (void) {
	_randseed = _randseed * 1103515245L + 12345L;
	return ((unsigned int) (_randseed >> 16) & RAND_MAX);
}
