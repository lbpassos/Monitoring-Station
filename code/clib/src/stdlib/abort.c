void abort() {

#ifdef __i386__
	asm ("int	$3");
#endif

#ifdef __arm__
	;
#endif

#ifdef __BORLANDC__
	asm {int	3}
#endif
	while (1)
		;
}
