# ifndef __ffly__crypto__h
# define __ffly__crypto__h
# include "ffint.h"
void ffly_frzz_enc(void*, ff_uint_t, ff_u64_t);
void ffly_frzz_dec(void*, ff_uint_t, ff_u64_t);
extern void(*ffly_encrypt)(void*, ff_uint_t, ff_u64_t);
extern void(*ffly_decrypt)(void*, ff_uint_t, ff_u64_t);
# endif /*__ffly__crypto__h*/
