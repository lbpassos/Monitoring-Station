#include <string.h>

char * strerror(int errnum)
{
	static char str[] = "A funcao \"strerror\" nao esta acabada\n";
	errnum = errnum;
	return str;
}
