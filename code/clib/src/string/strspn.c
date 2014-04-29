#include <string.h>

size_t strspn(const char *s1, const char *s2)
{
	int  i;
	const char *p;

	for (i = 0; *s1; s1++, i++) {
		for (p = s2; *p; p++)
			if (*s1 == *p)
				break;
		if (*p == 0)
			break;
	}
	return i;
}
