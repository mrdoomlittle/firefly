# ifndef __ffly__mode__h
# define __ffly__mode__h
# include <mdlint.h>
# define __ffmod_debug \
	if (ffis_mode(_ff_mod_debug))
enum {
	_ff_mod_def,
	_ff_mod_debug
};

void ffset_mode(mdl_i8_t);
mdl_u8_t ffis_mode(mdl_i8_t);
mdl_i8_t ffget_mode();
# endif /*__ffly__mode__h*/
