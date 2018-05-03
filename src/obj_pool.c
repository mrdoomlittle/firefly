# include "obj_pool.h"
# include "memory/mem_alloc.h"
# include "memory/mem_realloc.h"
# include "memory/mem_free.h"
ff_err_t ffly_obj_pool_init(ffly_obj_poolp __pool, ff_uint_t __size) {
    __pool->top = (ffly_objpp)__ffly_mem_alloc(__size*sizeof(ffly_objp));
    __pool->end = __pool->top;
}

ff_err_t ffly_obj_pool_de_init(ffly_obj_poolp __pool) {
    __ffly_mem_free(__pool->top);
}

ff_id_t ffly_obj_pool_add(ffly_obj_poolp __pool, ffly_objp __obj) {
    ff_id_t id = (ff_id_t)__ffly_mem_alloc(sizeof(__ff_id_t));
    *id = __pool->end-__pool->top;
    *(__pool->end++) = __obj;
    return id;
}

ff_err_t ffly_obj_pool_rm(ffly_obj_poolp __pool, ff_id_t __id) {
    if (__pool->top+*__id == __pool->end-1)
        __pool->end--;
    else
        *(__pool->top+*__id) = *(--__pool->end);
    __ffly_mem_free(__id);
}

ffly_objp ffly_obj_pool_get(ffly_obj_poolp __pool, ff_id_t __id) {
    return *(__pool->top+*__id);
}

void ffly_obj_pool_handle(ffly_unip __uni, ffly_obj_poolp __pool) {
    ffly_objpp itr = __pool->top;
    while(itr != __pool->end) {
        ffly_obj_handle(__uni, 0, *itr);
        itr++;
    }
}

struct ffly_obj_pool __ffly_obj_pool__;
