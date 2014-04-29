#include <string.h>

char * strpbrk(const char *s1, const char *s2)
{
	const char *p;

	for (; *s1; s1++) {
		for (p = s2; *p; p++)
			if (*s1 == *p)
				return (char *) s1;
	}
	return NULL;
}
