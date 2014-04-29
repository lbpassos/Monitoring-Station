#ifndef _STDARG_H
#define _STDARG_H

typedef char * va_list;

/*
	Dimensao de x arredondado para multiplo de sizeof(int)
*/

#define _size(x) ((sizeof(x) + sizeof(int)-1) & ~(sizeof(int)-1))

/*
	Assume-se que o buraco e' nos enderecos superiores

	va_arg(ap, char) num processador de 32 bits
	
	-----------------------------------------
	|	|car| *	| *	| *	|	|	|	|	|	|	|
	-----------------------------------------
		^ap				^ap + _size(char)
*/

#define va_start(p, arg)	(void) ((p) = (char *) & (arg) + _size(arg))
#define va_arg(p, T)		( * (T *) ( ((p) += _size(T)) - _size(T) ) )
#define va_end(p)			(void)0

#endif
