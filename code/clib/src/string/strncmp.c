#include <string.h>

int strncmp(const char * str1, const char * str2, size_t size) {
	int i;
	for(i = 0; i < size && *str1 && *str1 == *str2; str1++, str2++, ++i)
   		;
   	return (i == size) ? 0 : *str1 - *str2;
}
