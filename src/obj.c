# include "obj.h"
# include "memory/mem_free.h"
# include "memory/mem_alloc.h"
# include "system/errno.h"
# include "system/io.h"
# include "types/byte_t.h"
# include "data/bcopy.h"
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

ffly_err_t ffly_obj_draw(ffly_objp __obj, ffly_byte_t *__dst, mdl_uint_t __xa, mdl_uint_t __ya, mdl_uint_t __za, mdl_uint_t __xal, mdl_uint_t __yal, mdl_uint_t __zal, mdl_uint_t __xmax, mdl_uint_t __ymax, mdl_uint_t __zmax) {
    mdl_uint_t xa, ya, za = 0;
    while(za != __obj->zal && __obj->za+za < __zmax) {
        ya = 0;
        while(ya != __obj->yal && __obj->ya+ya < __ymax) {
            xa = 0;
            while(xa != __obj->xal && __obj->xa+xa < __xmax) {
                ffly_bcopy(__dst+(((__xa+xa)+((__ya+ya)*__yal))*4), __obj->texture+((xa+(ya*__obj->yal))*4), 4);
                xa++;
            }
            ya++;
        }
        za++;
    }
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
