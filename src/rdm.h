# ifndef __ffly__rdm__h
# define __ffly__rdm__h
# include "ffint.h"
/* resin dismantler */

void ffly_rdm(void(*__get)(ff_uint_t, ff_uint_t, void*), ff_u32_t, ff_u32_t);
void ffly_rdmp(ff_u8_t*, ff_u32_t);
void ffly_rdmf(char const*, ff_u32_t, ff_uint_t);
# endif /*__ffly__rdm__h*/
