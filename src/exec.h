# ifndef __ffly__exec__h
# define __ffly__exec__h
# include "ffint.h"
# include "resin/exec.h"
enum {
	_ffexec_bc,
	_ffexec_script
};

void ffexec(void(*)(ff_uint_t, ff_uint_t, void*), ff_u32_t, ff_u8_t, void(*)(void*, void*), void*, ff_u32_t);
void ffexecf(char const*);
# endif /*__ffly__exec__h*/
