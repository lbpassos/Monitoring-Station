#include <stdlib.h>
void * bsearch(const void * key, const void * base, size_t nelem, size_t size,
	int	 (* fcmp)(const void  *, const void	 *)) {
/*
	const char * p;
	size_t n;

	for (p = (const char *)base, n = nelem; n > 0; ) {
	const size_t pivot = n >> 1;
	const char * const q = p + size * pivot;
	const int val = (*fcmp)(key, q);

	if (val == 0)
		return ((void *) q);
	else if (val < 0)
		n = pivot;
	else {
		p = q + size;
		n = n - pivot - 1;
	}
	}
	return NULL;
*/
	while (nelem > 0) {
	const size_t pivot = nelem >> 1;
	const char * const q = (const char *)base + size * pivot;
	const int val = (*fcmp)(key, q);

	if (val == 0)
		return ((void *) q);
	else if (val < 0)
		nelem = pivot;
	else {
		base = q + size;
		nelem -= pivot + 1;
	}
	}
	return NULL;
}
