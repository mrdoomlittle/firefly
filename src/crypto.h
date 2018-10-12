# ifndef __ffly__crypto__h
# define __ffly__crypto__h
# include "ffint.h"

/*
	pass flags thru ffly_enc
	ffly_dec
*/
void ffly_frzz_enc(void*, ff_uint_t, ff_u64_t);
void ffly_frzz_dec(void*, ff_uint_t, ff_u64_t);
extern void(*ffly_encrypt)(void*, ff_uint_t, ff_u64_t);
extern void(*ffly_decrypt)(void*, ff_uint_t, ff_u64_t);

#define ff_encpt(...) \
	ffly_encrypt(__VA_ARGS__)
#define ff_decpt(...) \
	ffly_decrypt(__VA_ARGS__)
# endif /*__ffly__crypto__h*/
