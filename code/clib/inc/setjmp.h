#ifndef _SETJMP_H
#define _SETJMP_H

#if !defined(__CONFIG_H)
#include <_config.h>
#endif

#if 0
typedef struct {
	unsigned int	j_sp, j_ss,	j_flag, j_cs, j_ip, j_bp, j_di, j_es, j_si, j_ds;
} jmp_buf[1];
#endif

typedef int jmp_buf[23];

#ifdef __cplusplus
extern "C" {
#endif

void	longjmp(jmp_buf bufp, int retval);
int		setjmp(jmp_buf bufp);

#ifdef __cplusplus
}
#endif

#endif
