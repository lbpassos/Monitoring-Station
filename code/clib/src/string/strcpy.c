#include <string.h>

#define _C

char * strcpy(char * dst, const char * src) {
#ifdef _C
    char * aux = dst;
    while (*src)
        *dst++ = *src++;        
    *dst = *src;
    return aux;
#else
    return memcpy(dst, src, strlen(src));
#endif
}
