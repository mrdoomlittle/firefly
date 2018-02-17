# ifndef __ffly__put__bit__h
# define __ffly__put__bit__h
# include "types/err_t.h"
# include <mdlint.h>
typedef struct ffly_putbit {
    mdl_u8_t buf;
    mdl_u8_t left;
    void(*out)(mdl_u8_t);
} *ffly_putbitp;
# define ffly_init_put_bit(__putbit, __out) \
    (__putbit)->buf = 0x0; \
    (__putbit)->left = 8; \
    (__putbit)->out = out;

void ffly_put_bit(ffly_putbitp, mdl_u8_t, mdl_u8_t);
# endif /*__ffly__put__bit__h*/
