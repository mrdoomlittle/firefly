# include "obj.h"
# include "memory/mem_free.h"
# include "memory/mem_alloc.h"
# include "system/errno.h"
# include "system/io.h"
# include "dep/bcopy.h"
# include "dep/bzero.h"
/*
    using malloc for allocation is fine for now.
    it's only going to be slow when creating and destroying objs repeatedly.
*/
# define FASTSIZE 88
// needs testing
ffly_objp fastpool[FASTSIZE];
ffly_objp static *fast = fastpool;

ffly_objp static top = NULL;
ffly_objp static end = NULL;
void ffly_obj_rotate(ffly_objp __obj, float __angle) {
    __obj->angle = __angle;
}

ff_err_t ffly_obj_prepare(ffly_objp __obj) {
    __obj->angle = 0.0;
    __obj->lot = NULL;
    __obj->script = NULL;
    ffly_bzero(&__obj->shape, sizeof(ffly_polygon));
}

ffly_objp ffly_obj_alloc(ff_err_t *__err) {
    *__err = FFLY_SUCCESS;
    /*
        should have its own memory allocation region for this later,
        as malloc can be slow when calling lots of times.
    */

    ffly_objp obj;
    if (fast > fastpool)
        obj = *(--fast);
    else
        obj = (ffly_objp)__ffly_mem_alloc(sizeof(struct ffly_obj));
    ffly_fprintf(ffly_log, "alloced new object.\n");
    obj->next = NULL;
    obj->prev = NULL;
    obj->no = 0;

    if (!top) {
        top = obj;
    }

    if (end != NULL) {
        end->next = obj;
        obj->prev = end;
        obj->no = end->no+1;
    }
    end = obj;
    return obj;
}

ff_err_t ffly_obj_draw(ffly_objp __obj, ff_byte_t *__frame, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z, ff_uint_t __width, ff_uint_t __height, ff_uint_t __xmax, ff_uint_t __ymax) {
    ffly_draw_polygon(&__obj->shape, __frame, __obj->texture, __obj->xl, __x, __y, __z, __width, __height, __xmax, __ymax, __obj->angle);
}

ff_err_t ffly_obj_free(ffly_objp __obj) {
    ffly_fprintf(ffly_log, "freed object %u.\n", __obj->no);
    if (__obj == top) {
        top = __obj->next;
        if (top != NULL)
            top->prev = NULL;
    }

    if (__obj == end) {
        end = __obj->prev;
        if (end != NULL)
            end->next = NULL;
    } 

    __obj->next->prev = __obj->prev;
    __obj->prev->next = __obj->next;
    if (fast < fastpool+FASTSIZE)
        *(fast++) = __obj;
    else
        __ffly_mem_free(__obj);
    return FFLY_SUCCESS;
}

ff_err_t ffly_obj_cleanup() {
    ffly_fprintf(ffly_log, "cleaning up objects.\n");
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
    top = NULL;
    end = NULL;

    if (fast > fastpool) {
        ffly_objp *p = fastpool;
        while(p != fast) {
            __ffly_mem_free(*p);
            p++;
        }
    }
    fast = fastpool;
}

ff_err_t ffly_obj_handle(ffly_objp __obj) {

}
