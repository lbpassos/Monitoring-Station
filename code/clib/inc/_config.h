#if		defined(__LARGE__) || defined(__HUGE__) || defined(__COMPACT__)
#define _NULL	 0L
#else
#define _NULL	 0
#endif

#ifndef __SIZE_T
#define __SIZE_T

#if		defined(__BORLANDC__)

#if		defined(__LARGE__) || defined(__HUGE__) || defined(__COMPACT__)
typedef unsigned _size_t;	/* typedef unsigned long _size_t; */
#else
typedef unsigned _size_t;
#endif

#else

typedef unsigned long _size_t;

#endif

#endif

#ifndef __cplusplus
#ifndef __WCHAR_T
#define __WCHAR_T
typedef unsigned short _wchar_t;
#endif
#endif

#define _SIGNCHAR	1

#define INTLONG		0

#define _FNAME	80
#define _NFILE	10
#define _TNAME	13

#define _EDOM	33
#define _ERANGE 34
#define _EFPOS	35
#define _ERRMAX 36

#define _TBIAS	0

#define FIXPAR	pascal
