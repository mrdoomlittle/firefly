# include "obj.h"
# include "memory/mem_free.h"
# include "memory/mem_alloc.h"
# include "system/errno.h"
# include "system/io.h"
# include "types/byte_t.h"
# include "data/bcopy.h"
# include "data/bzero.h"
static ffly_objp top = NULL;
static ffly_objp end = NULL;
void ffly_obj_rotate(ffly_objp __obj, float __angle) {
    __obj->angle = __angle;
}

ffly_err_t ffly_obj_prepare(ffly_objp __obj) {
    __obj->angle = 0.0;
    __obj->lot = NULL;
    ffly_bzero(&__obj->shape, sizeof(ffly_polygon));
}

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

    if (end != NULL) {
        end->next = obj;
        obj->prev = end;
    }
    end = obj;
    return obj;
}

ffly_err_t ffly_obj_draw(ffly_objp __obj, ffly_byte_t *__frame, mdl_uint_t __x, mdl_uint_t __y, mdl_uint_t __z, mdl_uint_t __width, mdl_uint_t __height, mdl_uint_t __xmax, mdl_uint_t __ymax) {
    ffly_draw_polygon(&__obj->shape, __frame, __obj->texture, __x, __y, __z, __width, __height, __xmax, __ymax, __obj->angle);
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

ffly_err_t ffly_obj_cleanup() {
    ffly_objp obj = top, prev = NULL;
    while(obj != NULL) {
        if (prev != NULL)
            __ffly_mem_free(prev);
        /*
            free shit.
        */
        prev = obj;
        obj = obj->next;
    }

    if (end != NULL)
        __ffly_mem_free(end);
}

ffly_err_t ffly_obj_handle(ffly_objp __obj) {

}
