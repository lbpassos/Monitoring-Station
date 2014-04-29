#include <time.h>
#include <_time.h>

struct tm * gmtime(const time_t * time) {
	static struct tm t;
	_secstotm(&t, *time);
	return & t;
}
