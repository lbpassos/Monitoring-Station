#undef	assert

#ifdef	NDEBUG

#define assert(test)	((void)0)

#else

#ifdef __cplusplus
extern "C" {
#endif

void _assert(char *);

#ifdef __cplusplus
}
#endif

#define _VAL(x)	#x

#define _STR(x) _VAL(x)

#define assert(test) ((test) ? (void) 0 : _assert(__FILE__ ":" _STR(__LINE__) " " #test))

#endif
