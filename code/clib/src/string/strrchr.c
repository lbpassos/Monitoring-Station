#include <string.h>

char * strrchr(const char * str, int c) {
	int i;
	for(i = 0; *str; i++, str++)
		for( ; i > 0; i--, str--)
			if (*str == c)
				return (char *)str;
	return NULL;
}
