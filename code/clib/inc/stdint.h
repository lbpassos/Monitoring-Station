/*	Instituto Superior de Engenharia de Lisboa
	Ezequiel Conde, 2011
	
	Standard types definition to Cortex-M3
*/

#ifndef STDINT_H
#define STDINT_H

typedef unsigned char   uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned long   uint32_t;
typedef unsigned long long uint64_t;

typedef			 char   int8_t;
typedef 		 short  int16_t;
typedef 		 long   int32_t;
typedef			 long long int64_t;

#define	UINT64_MAX	18446744073709551615ULL
#define	UINT32_MAX	4294967295UL
#define	UINT16_MAX	65535U
#define UINT8_MAX	255

#define	INT32_MAX	2147483647L
#define	INT16_MAX	32767
#define INT8_MAX	127

#define	INT32_MIN	(-2147483648L)
#define	INT16_MIN	(-32768)
#define INT8_MIN	(-128)

#endif


