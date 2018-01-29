# ifndef __ffly__get__bits__h
# define __ffly__get__bits__h
# include "types/err_t.h"
struct ffly_getbit {
    mdl_u8_t buf;
    mdl_u8_t left;
    mdl_u8_t(*in)();
} *ffly_getbitp;
# define ffly_init_get_bits(__getbit, __in) \
    __getbit->buf = 0; \
    __getbit->left = 0; \
    __getbit->in = __in; 

mdl_u8_t ffly_get_bits(ffly_getbitp, mdl_u8_t);
# endif /*__ffly__get__bits__h*/
