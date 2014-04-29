#ifndef _DEFINES_
#define _DEFINES_

typedef void fvoid_t();

typedef long ptrdiff_t;

typedef unsigned long size_t;		/* Deve ter o mesmo numero de bits que ptrdiff_t */

typedef unsigned short wint_t;

enum capacity {default_size, reserve};

const size_t NPOS = (size_t) (-1);

#endif
