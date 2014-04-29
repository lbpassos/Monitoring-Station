#include <time.h>

time_t difftime(time_t t1, time_t t0) {
	return t1 >= t0 ? t1 - t0 : t0 - t1;
}
