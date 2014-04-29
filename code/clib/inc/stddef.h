#ifndef _STDDEF_H
#define _STDDEF_H

#include "_config.h"

#define	NULL	_NULL

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
#if		defined(__LARGE__) || defined(__HUGE__) || defined(__COMPACT__)
typedef long	ptrdiff_t;
#else
typedef int		ptrdiff_t;
#endif
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef _size_t size_t;
#endif

#define offsetof(T, member)	 ((size_t) & ((T *)0)->member)

#ifndef __cplusplus
#ifndef _WCHAR_T
#define _WCHAR_T
typedef _wchar_t	wchar_t;
#endif
#endif

#endif	/* STDDEF_H */
