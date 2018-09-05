# ifndef __ffly__resin__exec__h
# define __ffly__resin__exec__h
# include "../ffint.h"
# include "../tape.h"
void ffres_exec(void(*)(ff_uint_t, ff_uint_t, void*), ff_u32_t, void(*)(void*, void*), void*, ff_u32_t);
// exec tape
void ffres_xt(ffly_tapep, ff_u32_t);
# endif /*__ffly__resin__exec__h*/
