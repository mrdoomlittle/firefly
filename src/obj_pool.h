# ifndef __ffly__obj__pool__h
# define __ffly__obj__pool__h
# include "obj.h"
struct ffly_obj_pool {
    
};

typedef struct ffly_obj_pool* ffly_obj_poolp;
ffly_err_t ffly_obj_pool_init(ffly_obj_poolp, mdl_uint_t);
ffly_err_t ffly_obj_pool_de_init(ffly_obj_poolp);
extern struct ffly_obj_pool __ffly_obj_pool__;
# endif /*__ffly__obj__pool__h*/
