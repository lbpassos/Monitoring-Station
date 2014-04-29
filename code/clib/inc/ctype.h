#ifndef _CTYPE_H
#define _CTYPE_H

#define _IS_SP  0x01	/* is space */
#define _IS_DIG 0x02	/* is digit indicator */
#define _IS_UPP 0x04	/* is upper case */
#define _IS_LOW 0x08	/* is lower case */
#define _IS_HEX 0x10	/* [0..9] or [A-F] or [a-f] */
#define _IS_CTL 0x20	/* Control */
#define _IS_PUN 0x40	/* punctuation */
#define _IS_TAB 0x80	/* CR, FF, HT, NL, VT */

extern  char _ctype[];

#ifdef __cplusplus
extern "C" {
#endif
int	isalnum (int c);
int	isalpha (int c);
int	iscntrl (int c);
int	isdigit (int c);
int	isgraph (int c);
int	islower (int c);
int	isprint (int c);
int	ispunct (int c);
int	isspace (int c);
int	isupper (int c);
int	isxdigit(int c);
int tolower(int c);
int toupper(int c);
#ifdef __cplusplus
}
#endif

#define isalnum(c)	(_ctype[(c) + 1] & (_IS_DIG | _IS_UPP | _IS_LOW))
#define isalpha(c)	(_ctype[(c) + 1] & (_IS_UPP | _IS_LOW))
#define iscntrl(c)	(_ctype[(c) + 1] & _IS_CTL)
#define isdigit(c)	(_ctype[(c) + 1] & _IS_DIG)
#define isgraph(c)	(_ctype[(c) + 1] & (_IS_DIG | _IS_LOW | _IS_UPP | _IS_PUN))
#define islower(c)	(_ctype[(c) + 1] & _IS_LOW)
#define isprint(c)	(_ctype[(c) + 1] & (_IS_DIG | _IS_LOW | _IS_UPP | _IS_PUN | _IS_SP))
#define ispunct(c)	(_ctype[(c) + 1] & _IS_PUN)
#define isspace(c)	(_ctype[(c) + 1] & (_IS_SP | _IS_TAB))
#define isupper(c)	(_ctype[(c) + 1] & _IS_UPP)
#define isxdigit(c) (_ctype[(c) + 1] & (_IS_DIG | _IS_HEX))

#define	EOF		(-1)

/* Definicoes nao Standard */

#if !__STDC__

#ifdef __cplusplus
extern "C" {
#endif
int isascii (int c);
int toascii (int c);
#ifdef __cplusplus
}
#endif

#define isascii(c)	((unsigned)(c) < 128)
#define toascii(c)	((c) & 0x7f)

#define _toupper(c) ((c) + 'A' - 'a')
#define _tolower(c) ((c) + 'a' - 'A')

#endif

#endif
