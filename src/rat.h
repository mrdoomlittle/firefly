# ifndef __ffly__rat__h
# define __ffly__rat__h
# include "ffint.h"
# include <stdarg.h>

enum {
	_ff_rat_0,
	_ff_rat_1,
	_ff_rat_2
};

void ff_rat(ff_u8_t, void(*)(char const*, va_list), char const*, ...);
void ff_rat_put(ff_u8_t);
# endif /*__ffly__rat__h*/
