# ifndef __ffly__put__bits__h
# define __ffly__put__bits__h
# include "types/err_t.h"
struct ffly_putbit {
    mdl_u8_t buf;
    mdl_u8_t size;
    void(*out)(mdl_u8_t);
} *ffly_putbitp;
# define ffly_init_put_bits(__putbit, __out) \
    __putbit->buf = 0; \
    __putbit->size = 0; \
    __putbit->out = out;

void ffly_put_bits(ffly_putbitp, mdl_u8_t, mdl_u8_t);
# endif /*__ffly__put__bits__h*/
