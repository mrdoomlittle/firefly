# include "cache.h"
# include "system/errno.h"
# include "memory/mem_free.h"
# include "memory/mem_alloc.h"
# include "ffly_def.h"
struct record {
    void *p;
    ffly_id_t id;

    struct record *fd;
};

typedef struct record* recordp;
typedef recordp* recordpp;
typedef recordpp* recordppp;
mdl_uint_t static max;
recordp static top = NULL;
recordp static fresh;

recordp bin = NULL;
void ffly_set_cache_dir(char *__dir) {
}

ffly_err_t ffly_cache_prepare(mdl_uint_t __max) {
    top = (recordp)__ffly_mem_alloc(__max*sizeof(struct record));
    fresh = top; 
    max = __max;
}

ffly_err_t ffly_cache_put(ffly_id_t *__id, void *__p) {
    if (fresh>=top+max) {
        return FFLY_FAILURE;
    }
 
    ffly_id_t id;

    if (bin != NULL) {
        id = bin->id;
        bin = bin->fd;
    } else
        id = (ffly_id_t)__ffly_mem_alloc(sizeof(__ffly_id_t));
    recordp rec;
    *id = fresh-top;
    *(rec = fresh++) = (struct record) {
        .id = id,
        .p = __p
    };

    *__id = id;
    return FFLY_SUCCESS;
}

void* ffly_cache_get(ffly_id_t __id, ffly_err_t *__err) {
    *__err = FFLY_SUCCESS;
}

ffly_err_t ffly_cache_del(ffly_id_t __id) {
    recordp rec = top+*__id;
    if (rec == fresh-1) {
        fresh--;
        return FFLY_SUCCESS;
    }
    
    rec->fd = NULL;
    if (!bin)
        bin = rec;
    else {
        rec->fd = bin;
        bin = rec;
    }
    return FFLY_SUCCESS;
}


ffly_err_t ffly_cache_free() {
    __ffly_mem_free(top);
}
/*
# include <stdio.h>
int main() {
    ffly_cache_prepare(20);
    ffly_id_t id;
    while(1) {
    ffly_cache_put(&id, (void*)123);
    ffly_cache_del(id);
    }
}*/
