char * _longtoa(long, char *, int, int);

char * itoa(int value, char * str, int radix) {

	return _longtoa (
		(radix == 10) ? (long) value : (unsigned long)((unsigned)value),
		str, radix, (radix == 10));
}

char * ultoa(unsigned long value, char * str, int radix) {
	return	_longtoa (value, str, radix, 0);
}

char * ltoa(long value, char * str, int radix) {
	return	_longtoa (value, str, radix, (radix == 10));
}
