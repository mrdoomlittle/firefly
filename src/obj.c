# include "obj.h"
# include "memory/mem_free.h"
# include "memory/mem_alloc.h"
# include "system/errno.h"
# include "system/io.h"
static ffly_objp top = NULL;
static ffly_objp end = NULL;
ffly_objp ffly_obj_alloc(ffly_err_t *__err) {
    *__err = FFLY_SUCCESS;
    /*
        should have its own memory allocation region for this later,
        as malloc can be slow when calling lots of times.
    */

    ffly_objp obj = (ffly_objp)__ffly_mem_alloc(sizeof(struct ffly_obj));
    ffly_fprintf(ffly_log, "alloced new object.\n");
    obj->next = NULL;
    obj->prev = NULL;

    if (!top) {
        top = obj;
    }

    if (!end) {
        end = obj;
    } else {
        end->next = obj;
        obj->prev = end;
        end = obj;
    }
    return obj;
}

ffly_err_t ffly_obj_free(ffly_objp __obj) {
    ffly_fprintf(ffly_log, "freed object.\n");
    if (__obj == top) {
        top = __obj->next;
    }

    if (__obj == end) {
        end = __obj->prev;
    } 

    __obj->next->prev = __obj->prev;
    __obj->prev->next = __obj->next;
    __ffly_mem_free(__obj);
    return FFLY_SUCCESS;
}

ffly_err_t ffly_obj_handle(ffly_objp __obj) {

}
