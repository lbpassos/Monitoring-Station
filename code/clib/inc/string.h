#ifndef _STRING_H
#define _STRING_H

#ifndef __CONFIG_H
#include <_config.h>
#endif

#define NULL	_NULL

#ifndef _SIZE_T
#define _SIZE_T
typedef _size_t size_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void * memchr(const void * s, int c, size_t size);
int    memcmp(const void  *s1, const void *s2, size_t size);
void * memcpy (void *dst, const void *src, size_t size);
void * memmove(void *dst, const void *src, size_t size);
void * memset(void *s, int c, size_t size);

char * strcat(char *dst, const char *src);
char * strchr(const char * s, int c);
int    strcmp(const char *s1, const char *s2);
char * strcpy(char *dst, const char *src);
size_t strcspn(const char *s1, const char *s2);
char * strerror(int errnum);
size_t strlen(const char *s);
char * strncat(char *dst, const char *src, size_t malen);
int    strncmp(const char *s1, const char *s2, size_t malen);
char * strncpy(char *dst, const char *src, size_t malen);
char * strpbrk(const char *s1, const char *s2);
char * strrchr(const char *s, int c);
size_t strspn(const char *s1, const char *s2);
char * strstr(const char *s1, const char *s2);
char * strtok(char *s1, const char *s2);

#ifdef __cplusplus
}
#endif

#endif  /* _STRING_H */
