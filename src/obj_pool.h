# ifndef __ffly__obj__pool__h
# define __ffly__obj__pool__h
# include "ffint.h"
# include "types.h"
# include "obj.h"
# include "uni.h"
/*
	should rename.
	and x% of objects will be in a pool with other objects
	being handled by the pools threads as eatch pool has its own.
*/
struct ffly_obj_pool {
    ffly_objpp top, end;
};

typedef struct ffly_obj_pool* ffly_obj_poolp;
ff_err_t ffly_obj_pool_init(ffly_obj_poolp, ff_uint_t);
ff_err_t ffly_obj_pool_de_init(ffly_obj_poolp);
ff_id_t ffly_obj_pool_add(ffly_obj_poolp, ffly_objp);
ff_err_t ffly_obj_pool_rm(ffly_obj_poolp, ff_id_t);
ffly_objp ffly_obj_pool_get(ffly_obj_poolp, ff_id_t);
void ffly_obj_pool_handle(ffly_unip, ffly_obj_poolp);
extern struct ffly_obj_pool __ffly_obj_pool__;
# endif /*__ffly__obj__pool__h*/
