#ifndef _ERRNO_H
#define _ERRNO_H

#if !defined(__CONFIG_H)
#include <_config.h>
#endif

#define	EDOM	_EDOM
#define ERANGE	_ERANGE
#define EFPOS	_EFPOS

#define _NERR	_ERRMAX

extern int errno;

#endif
