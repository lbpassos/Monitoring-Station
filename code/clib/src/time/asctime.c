#include <time.h>
#include "_time.h"

char * asctime(const struct tm * t) {
	static char buf[81] = "Day Mon dd hh:mm:ss yyyy\n";
	_strftime(buf, sizeof(buf), "%a %c\n", t);
	return buf;
}


