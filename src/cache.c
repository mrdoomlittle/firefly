# include "cache.h"
# include "system/errno.h"
# include "memory/mem_free.h"
# include "memory/mem_alloc.h"
struct item {
    void *p; 
};

struct item *items = NULL;
struct item *fresh;

void ffly_set_cache_dir(char *__dir) {
}


ffly_err_t ffly_cache_prepare(mdl_uint_t __max_items) {
    items = (struct item*)__ffly_mem_alloc(__max_items*sizeof(struct item));
    fresh = items;
}


ffly_err_t ffly_cache_put(ffly_no_t *__no, void *__p) {

}

void* ffly_cache_get(ffly_no_t __no, ffly_err_t *__err) {
    *__err = FFLY_SUCCESS;
}

ffly_err_t ffly_cache_del(ffly_no_t __no) {

}


ffly_err_t ffly_cache_free() {
    __ffly_mem_free(items);
}
