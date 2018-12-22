# ifndef __ffly__mode__h
# define __ffly__mode__h
# include "ffint.h"
#define __ffmod_debug \
	if (ffis_mode(_ff_mod_debug))
enum {
	_ff_mod_def,
	_ff_mod_debug,
};

void ffset_mode(ff_i8_t);
ff_u8_t ffis_mode(ff_i8_t);
ff_i8_t ffget_mode(void);
# endif /*__ffly__mode__h*/
