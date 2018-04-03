# ifndef __ffly__crypto__h
# define __ffly__crypto__h

void ffly_frzz_enc(void*, mdl_uint_t, mdl_u64_t);
void ffly_frzz_dec(void*, mdl_uint_t, mdl_u64_t);
extern void(*ffly_encrypt)(void*, mdl_uint_t, mdl_u64_t);
extern void(*ffly_decrypt)(void*, mdl_uint_t, mdl_u64_t);
# endif /*__ffly__crypto__h*/
