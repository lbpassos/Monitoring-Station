#include <stdlib.h>

unsigned long _randseed = 1;

void srand (unsigned int seed) {
	_randseed = seed;
}
