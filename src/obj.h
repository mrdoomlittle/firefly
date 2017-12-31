# ifndef __ffly__obj__h
# define __ffly__obj__h
# include "types/err_t.h" 
# include "graphics/image.h"
struct ffly_obj {
    ffly_imagep texture;
    mdl_uint_t xal, yal, zal;
    mdl_uint_t xa, ya, xa;
};


ffly_err_t ffly_obj_update(struct ffly_obj*);
# endif /*__ffly__obj__h*/
