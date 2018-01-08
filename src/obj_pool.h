# ifndef __ffly__obj__pool__h
# define __ffly__obj__pool__h
# include "obj.h"
# include <mdlint.h>
# include "types/id_t.h"
struct ffly_obj_pool {
    ffly_objpp top, end;
};

typedef struct ffly_obj_pool* ffly_obj_poolp;
ffly_err_t ffly_obj_pool_init(ffly_obj_poolp, mdl_uint_t);
ffly_err_t ffly_obj_pool_de_init(ffly_obj_poolp);
ffly_id_t ffly_obj_pool_add(ffly_obj_poolp, ffly_objp);
ffly_err_t ffly_obj_pool_rm(ffly_obj_poolp, ffly_id_t);
ffly_objp ffly_obj_pool_get(ffly_obj_poolp, ffly_id_t);
void ffly_obj_pool_handle(ffly_obj_poolp);
extern struct ffly_obj_pool __ffly_obj_pool__;
# endif /*__ffly__obj__pool__h*/
