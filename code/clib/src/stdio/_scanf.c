#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <_stdio.h>
#include <errno.h>

static int _getint(_Sft * px, char code) {
	char ac[FMAX + 1], *p;
	char seendig = 0;
	int ch;
	static const char digits[] = "0123456789abcdefABCDEF";
	static const char flit[] = "diouxXp";
	static const char bases[] = {10, 0, 8, 10, 16, 16, 16};
	int base = bases[(const char *) strchr(flit, code) - flit];
	int dlen;

	px->nget = px->width <= 0 || FMAX <px->width ? FMAX : px->width;
	p = ac, ch = GETN(px);
	if (ch == '+' || ch == '-')
		*p++ = ch, ch = GETN(px);
	if (ch == '0') {
		seendig = 1;
		*p++ = ch, ch = GETN(px);
		if ((ch == 'x' || ch == 'X') && (base == 0 || base == 16))
			base = 16, *p++ = ch, ch = GETN(px);
		else
			base = 8;
	}
	dlen = base == 0 || base == 10 ? 10 : base == 8 ? 8 : 16 + 6;
	for (; memchr(digits, ch, dlen); seendig = 1)
		*p++ = ch, ch = GETN(px);
	UNGETN(px, ch);
	if (!seendig)
		return -1;
	*p = '\0';
	if (px->noconv)
		;
	else if (code == 'd' || code == 'i') {
		long lval = strtol(ac, NULL, base);
		if (errno == ERANGE)
			return -1;
		px->stored = 1;
		if (px->qual == 'h')
			*va_arg(px->ap, short *) = lval;
		else if (px->qual == 'l')
			*va_arg(px->ap, int *) = lval;
		else
			*va_arg(px->ap, long *) = lval;
	}
	else {
		unsigned long ulval = strtoul(ac, NULL, base);

		px->stored = 1;
		if (code == 'p')
			*va_arg(px->ap, void * *) = (void *) ulval;
		else if (px->qual == 'h')
			*va_arg(px->ap, unsigned short *) = ulval;
		else if (px->qual == 'l')
			*va_arg(px->ap, unsigned int *) = ulval;
		else
			*va_arg(px->ap, unsigned long *) = ulval;
	}
	return 0;
}	
	
static const char * _getfld(_Sft * px, const char * s) {
	int ch;
	char * p;
	px->stored = 0;
	switch (*s) {
		case 'c':
			if (px->width == 0)
				px->width = 1;
			p = va_arg(px->ap, char *);
			for (; px->width > 0; --px->width)
				if ((ch = GET(px)) < 0)
					return NULL;
				else if ( ! px->noconv) {
					*p++ = ch;
					px->stored = 1;
				}
			++s;
			break;

		case 'P':
		case 'd': case 'i': case 'o':
		case 'u': case 'x': case 'X':
			if (_getint(px, *s))
				return NULL;
			++s;
			break;
		case 'e': case 'E': case 'f':
		case 'g': case 'G':
			++s;
			return NULL;
		case 'n':
			if (px->qual == 'h')
				*va_arg(px->ap, short *) = px->nchar;
			else if (px->qual == 'l')
				*va_arg(px->ap, int *) = px->nchar;
			else
				*va_arg(px->ap, long *) = px->nchar;
			++s;
			break;

		case 's':
			px->nget = px->width <= 0 ? INT_MAX : px->width;
			p = va_arg(px->ap, char *);
			while ((ch = GETN(px)) >= 0)
				if (isspace(ch))
					break;
				else if (!px->noconv)
					*p++ = ch;
			UNGETN(px, ch);
			if ( ! px->noconv)
				*p++ = '\0', px->stored = 1;
			++s;
			break;

		case '%':
			++s;
			if ((ch = GET(px)) == '%')
				break;
			UNGET(px, ch);
			return NULL;

		case '[': {
			char comp = *++s == '^' ? *s++ :'\0';
			const char *t = strchr(*s == ']' ? s + 1 : s, ']');
			size_t n = t - s;

			if (t == NULL)
				return NULL;
			px->nget = px->width <= 0 ? INT_MAX : px->width;
			p = va_arg(px->ap, char *);
			while ((ch = GETN(px)) >= 0)
				if ((! comp && ! memchr(s, ch, n)) || (comp && memchr(s, ch, n)))
					break;
				else if (!px->noconv)
					*p++ = ch;
			UNGETN(px, ch);
			if (!px->noconv)
				*p++ = '\0', px->stored = 1;
			s = t;
			break;
		}
		default:
			return NULL;
	}
	return s;
}

int _scanf(int (*pfn)(void *, int), void * arg, const char * fmt, va_list ap) {
	const char * s;
	int nconv = 0;
	_Sft x;
	int ch;	
		
	x.pfn = pfn;
	x.arg = arg;
	x.ap = ap;
	x.nchar = 0;
	for (s = fmt; ; ) {
		while (1) {
			if (*s == '%') {
				++s;
				break;
			}

			if (*s == '\0')
				return nconv;

			if (isspace(*s)) {
				do {
					++s;
				} while (isspace(*s));
				while(isspace(ch = GET(&x)))
					;
				UNGET(&x, ch);
				continue;
			}
			ch = GET(&x);
			if (ch == *s)
				++s;
			else
				return nconv;
		}
		x.noconv = *s == '*' ? *s++ : '\0';
		for (x.width = 0; isdigit(*s); ++s)
			if (x.width < _WMAX)
				x.width = x.width * 10 + *s - '0';
		x.qual = strchr("hlL", *s) ? *s++ : 'l';
		if (!strchr("cn[", *s)) {
			while (isspace(ch = GET(&x)))
				;
			UNGET(&x, ch);
		}
		if ((s = _getfld(&x, s)) == NULL)
			return nconv > 0 ? nconv : EOF;
		if (x.stored)
			++nconv;
	}
}
			
