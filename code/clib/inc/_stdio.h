#ifndef __STDIO_H
#define __STDIO_H

#include <stdarg.h>
#include <stdio.h>

#define	_FSP	0x01
#define	_FPL	0x02
#define	_FMI	0x04
#define	_FNO	0x08
#define	_FZE	0x10

#define	_WMAX	999
#define _WANT	(EOF - 1)

#define FMAX	512

#define GET(px)			(++(px)->nchar, (*(px)->pfn)((px)->arg, _WANT))
#define GETN(px)		(--(px)->nget >= 0 ? GET(px) : _WANT)
#define UNGET(px, ch)	(--(px)->nchar, (*(px)->pfn)((px)->arg, ch))
#define UNGETN(px, ch)	{ if ((ch) != _WANT) UNGET(px, ch); }

typedef struct {
	int (*pfn) (void *, int);
	void * arg;
	va_list ap;
	int nchar, nget, width;
	char noconv, qual, stored;
} _Sft;

#ifdef __cplusplus
extern "C" {
#endif

int _printf(void * ( * pfn) (void *, const char *, size_t),
	void * arg, size_t count, const char * fmt, va_list ap);

int _scanf(int ( * pfn) (void *, int),
	void * arg, const char * fmt, va_list ap);

#ifdef __cplusplus
}
#endif

#endif
