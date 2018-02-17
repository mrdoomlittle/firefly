# ifndef __ffly__get__bit__h
# define __ffly__get__bit__h
# include "types/err_t.h"
# include <mdlint.h>
typedef struct ffly_getbit {
    mdl_u8_t buf;
    mdl_u8_t left;
    mdl_u8_t(*in)(void*);
	void *arg_p;
} *ffly_getbitp;
# define ffly_init_get_bit(__getbit, __in, __arg_p) \
    (__getbit)->buf = 0x0; \
    (__getbit)->left = 0; \
    (__getbit)->in = __in; \
	(__getbit)->arg_p = __arg_p

mdl_u8_t ffly_get_bit(ffly_getbitp, mdl_u8_t);
# endif /*__ffly__get__bit__h*/
