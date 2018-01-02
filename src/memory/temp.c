# include "temp.h"
# include "mem_alloc.h"
# include "mem_free.h"
# define TEMP_SSIZE 400
static void *p = NULL;
static void *fresh = NULL;
void* ffly_temp_alloc(mdl_uint_t __size, ffly_err_t *__err) {
    if (!p)
        p = __ffly_mem_alloc(TEMP_SSIZE);
    return fresh+__size;
}

ffly_err_t ffly_temp_free(void *__p) {
    if (fresh < __p && __p >= p) 
        fresh = __p;
}
