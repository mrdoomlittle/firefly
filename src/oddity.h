# ifndef __ffly__oddity__h
# define __ffly__oddity__h
# include "ffint.h"
/*
	anything out of the ordinary
	errors/warnings/etc...
*/
extern ff_u32_t oddity_count;
/*
	if odditys exceed x amount then inform the user or panic and begin shutdown procedures
*/
#ifdef ignore_odditys
#define caught_oddity
#else
#define caught_oddity\
	__asm__("lock incl %0" : "=m"(oddity_count))
#endif
# endif /*__ffly__oddity__h*/
