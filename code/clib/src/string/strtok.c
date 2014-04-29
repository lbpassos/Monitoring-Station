#include <string.h>

char * strtok(char *s1, const char *s2)
{
	static char * p;
	char * p1, * aux;
	const char * p2;

	if (s1 != 0)
		p = s1;
	if (*s2 == '\0')
		return p;
	if (*p == '\0')
		return 0;
	for (p1 = p; *p1; p1++) {
		for (p2 = s2; *p2; p2++)
			if (*p1 == *p2) {
				*p1 = 0;
				aux = p;
				p = p1 + 1;
				return aux;
			}
	}
	return p;
}
