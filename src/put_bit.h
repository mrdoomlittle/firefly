# ifndef __ffly__put__bit__h
# define __ffly__put__bit__h
# include "ffint.h"
# include "types.h"
typedef struct ffly_putbit {
    ff_u8_t buf;
    ff_u8_t left;
    void(*out)(ff_u8_t);
} *ffly_putbitp;
# define ffly_init_put_bit(__putbit, __out) \
    (__putbit)->buf = 0x0; \
    (__putbit)->left = 8; \
    (__putbit)->out = out;

void ffly_put_bit(ffly_putbitp, ff_u8_t, ff_u8_t);
# endif /*__ffly__put__bit__h*/
