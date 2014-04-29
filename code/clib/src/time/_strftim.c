#include <time.h>
#include <string.h>
#include "_time.h"

#define SUNDAY	0
#define MONDAY	1

static char * getval(char * s, int val, int n) {
	if (val < 0)
		val = 0;
	for (s += n, *s = '\0'; --n >= 0; val /= 10)
		*--s = val % 10 + '0';
	return s;
}

static int wkyr(int wstart, int wday, int yday) {
	wday = (wday + 7 - wstart) % 7;
	return (yday - wday + 12) / 7 - 1;
}

static const char * _getfield(const char * pstart, int posit, int * len) {
	const char delim = *pstart ? *pstart++ : '\0';
	const char * pend;

	for( ; ; --posit, pstart = pend + 1) {
		if ((pend = strchr(pstart, delim)) == NULL)
			pend = pstart + strlen(pstart);
		if (posit <= 0) {
			*len = pend - pstart;
			return pstart;
		}
		else if (*pend == '\0') {
			*len = 1;
			return pend;
		}
	}
}

const char * _gentime(const struct tm * t,
					  const char * s, int * pn , char * ac) {
	const char * p;

	switch(*s) {

	case 'a':
		p = _getfield(tin_getDays(), t->tm_wday << 1, pn);
		break;
	case 'A':
		p = _getfield(tin_getDays(), (t->tm_wday << 1) + 1, pn);
		break;
	case 'b':
		p = _getfield(tin_getMonths(), t->tm_mon << 1, pn);
		break;
	case 'B':
		p = _getfield(tin_getMonths(), (t->tm_mon << 1) + 1, pn);
		break;
	case 'c':
		p = _getfield(tin_getFormats(), 0, pn); *pn = -*pn;
		break;
	case 'd':
		p = getval(ac, t->tm_mday, *pn = 2);
		break;
	case 'D':
		p = getval(ac, t->tm_mday, *pn = 2);
		if (ac[0] == '0')
			ac[0] = ' ';
		break;
	case 'H':
		p = getval(ac, t->tm_hour, *pn = 2);
		break;
	case 'I':
		p = getval(ac, t->tm_hour % 12, *pn = 2);
		break;
	case 'j':
		p = getval(ac, t->tm_yday + 1, *pn = 3);
		break;
	case 'm':
		p = getval(ac, t->tm_mon + 1, *pn = 2);
		break;
	case 'M':
		p = getval(ac, t->tm_min, *pn = 2);
		break;
	case 'p':
		p = _getfield(tin_getAmPm(), 12 <= t->tm_hour, pn);
		break;
	case 'S':
		p = getval(ac, t->tm_sec, *pn = 2);
		break;
	case 'U':
		p = getval(ac, wkyr(SUNDAY, t->tm_wday, t->tm_yday), *pn = 2);
		break;
	case 'w':
		p = getval(ac, t->tm_wday, *pn = 1);
		break;
	case 'W':
		p = getval(ac, wkyr(MONDAY, t->tm_wday, t->tm_yday), *pn = 2);
		break;
	case 'x':
		p = _getfield(tin_getFormats(), 1, pn), *pn = -*pn;
		break;
	case 'X':
		p = _getfield(tin_getFormats(), 2, pn), *pn = -*pn;
		break;
	case 'y':
		p = getval(ac, t->tm_year % 100, *pn = 2);
		break;
	case 'Y':
		p = getval(ac, t->tm_year + 1900, *pn = 4);
		break;
	case 'Z':
		p = _getfield(tin_getTzone(), t->tm_isdst > 0, pn);
		break;
	case '%':
		p = "%", *pn = 1;
		break;
	default:
		p = s - 1, *pn = 2;
	}
	return p;
}

#define PUT(s, na)	\
		(void)( nput = (na), \
				nput > 0 && (nchar += nput) <= bufsize ? \
					(memcpy(buf, s, nput), buf +=nput) : 0 ) \

size_t _strftime(char * buf, size_t bufsize, const char * fmt,
					const struct tm * t) {
	const char * s;

	size_t nput, nchar = 0;
	int lensav, len = strlen(fmt);
	const char * fmtsav = NULL;
	
	for ( ; ; ) {
		const char * s;
	
		for (s = fmt; *s != '%' && len > 0; ++s, --len)
			;
		PUT(fmt, s - fmt);
		if (len == 0 && fmtsav == NULL) {
			PUT("", 1);
			return nchar - 1;
		}
		else if (len == 0) { /* && fmtsav != NULL) */
			fmt = fmtsav, fmtsav = NULL, len = lensav;
			continue;
		}	
		/* ------------------------------------------------------------- */

		/* parou por '%', processar um campo de formatacao */ 
		{
			char ac[20];
			int m;
			const char *p = _gentime(t, s + 1, &m, ac);
			s += 2, len -=2;
			if (m >= 0)
				PUT(p, m);
			else if (fmtsav == NULL)
				fmtsav = s, lensav = len, s = p, len = -m;
		}
		fmt = s;
	}
}

