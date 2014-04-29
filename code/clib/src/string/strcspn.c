#include <string.h>

size_t strcspn(const char *s1, const char *s2)
{
	int  i;
	const char *p;

	for (i = 0; *s1; s1++, i++) {
		for (p = s2; *p; p++)
			if (*s1 == *p)
				goto break2;
	}
	break2:
	return i;
}
