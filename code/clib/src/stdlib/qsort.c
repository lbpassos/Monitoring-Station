#include <stdlib.h>
#include <string.h>

#define MAX_BUF	256

#define min(a,b)	((a) < (b) ? a : b)

void _change(char * p1, char * p2, size_t size);

void qsort(void * base, size_t n, size_t size,
	int  (* fcmp)(const void  *, const void  *)) {
	int i = 0, j = n - 1;
	char * qi = (char *)base;
	char * qj = (char *)base + size * j;
	char * qp = (char *)base + size * (j >> 1);		/* j / 2 */
	while (i <= j) {
		while ((*fcmp)(qi, qp) < 0)
				++i, qi += size;
		while ((*fcmp)(qp, qj) < 0)
				--j, qj -= size;
		if (i <= j) {
			_change(qi, qj, size);
			++i, qi += size, --j, qj -= size;
		}
	}
	if (j > 0)
		qsort(base, j + 1, size, fcmp);
	if (i < n - 1)
		qsort(qi, n - i, size, fcmp);
}
