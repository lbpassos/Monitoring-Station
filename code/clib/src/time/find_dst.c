/* This is a transcription of a Perl5 script by Lionel Cons,CN *
 * into C source code.                Dietrich Wiegandt 960828 */

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
char *getenv();

main()
{
int n = 20;                     /* number of weeks to check */
int dst, ndst, i;
time_t week = 60*60*24*7;       /* one week in seconds */
time_t now, mid, min, max;

	now = time(NULL);           /* now */
	dst = localtime(&now)->tm_isdst;    /* current dst state */
	printf ("Today is %.24s, DST is %s.\n", ctime(&now), (dst ? "on" : "off"));
	printf ("The current value of TZ is %s.\n", (getenv("TZ") != NULL ? getenv("TZ") : "not set"));

/* find the week of the dst change */
	for(i = 1,ndst = dst; (ndst == dst) && (i <= n); i++)  {
		now += week;
		ndst = localtime(&now)->tm_isdst;
	}
	if (ndst == dst) {
		printf("No DST change during the next %d weeks\n", n);
		exit();
	}
/* find the exact time */

	min = now - week;
	max = now;
	while (max - min > 1)   {
		mid = (max + min) >> 1;
		ndst = localtime(&mid)->tm_isdst;
		if (ndst == dst) min=mid; else max=mid;
	}
/* print the result */
printf ("The next DST change will be %s", ctime(&min));
printf ("one second later it will be %s", ctime(&max));
}
