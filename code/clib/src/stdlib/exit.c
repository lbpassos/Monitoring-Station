#include <stddef.h>

typedef void (* Function)(void);

static Function tab[10];

static size_t index = 0;

void exit(int status) {
	while (index > 0)
		tab[--index]();
	while (1)
		;
}

int atexit(void ( *func)(void)) {
	if (index == sizeof(tab)/sizeof(tab[0]))
		return -1;
	tab[index++] = func;	
	return 0;
}
