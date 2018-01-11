# include "lot.h"
# include "system/io.h"
# include "system/errno.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
static ffly_lotp top = NULL;
static ffly_lotp end = NULL;

void ffly_lot_add(ffly_lotp __lot, ffly_objp __obj) {
    mdl_uint_t xoff = __obj->x-__lot->x;
    mdl_uint_t yoff = __obj->y-__lot->y;
    mdl_uint_t zoff = __obj->z-__lot->z;
    *(*(__lot->objs+xoff+(yoff*__lot->yl)+(zoff*(__lot->yl*__lot->xl))) = __lot->end++) = __obj;
}

void ffly_lot_rm(ffly_lotp __lot, ffly_objp __obj) {
    if (!__lot || !__obj) {
        ffly_fprintf(ffly_err, "failed to remove from lot.\n");
        return;
    }

    mdl_uint_t xoff = __obj->x-__lot->x;
    mdl_uint_t yoff = __obj->y-__lot->y;
    mdl_uint_t zoff = __obj->z-__lot->z;
    ffly_objppp obj = __lot->objs+xoff+(yoff*__lot->yl)+(zoff*(__lot->yl*__lot->xl)); 
    if (*obj == __lot->end-1)
        __lot->end--;
    else
        **obj = *(--__lot->end);
}

ffly_objpp ffly_lot_obj(ffly_lotp __lot, ffly_objp __obj) {
    mdl_uint_t xoff = __obj->x-__lot->x;
    mdl_uint_t yoff = __obj->y-__lot->y;
    mdl_uint_t zoff = __obj->z-__lot->z;
    return *(__lot->objs+xoff+(yoff*__lot->yl)+(zoff*(__lot->yl*__lot->xl)));
}

void ffly_lot_prepare(ffly_lotp __lot, mdl_uint_t __x, mdl_uint_t __y, mdl_uint_t __z) {
    __lot->x = __x;
    __lot->y = __y;
    __lot->z = __z;
}

ffly_lotp ffly_alloc_lot(mdl_uint_t __xl, mdl_uint_t __yl, mdl_uint_t __zl) {
    ffly_lotp lot = (ffly_lotp)__ffly_mem_alloc(sizeof(struct ffly_lot));
    lot->next = NULL;
    lot->prev = NULL;
    if (!top) {
        top = lot;
    }

    if (end != NULL) { 
        end->next = lot;
        lot->prev = end;
    }

    end = lot;

    mdl_uint_t size = (lot->xl = __xl)*(lot->yl = __yl)*(lot->zl = __zl);
    lot->top = (ffly_objpp)__ffly_mem_alloc(size*sizeof(ffly_objp));
    lot->end = lot->top;

    lot->objs = (ffly_objppp)__ffly_mem_alloc(size*sizeof(ffly_objpp));
    ffly_objppp itr = lot->objs;
    while(itr != lot->objs+size) *(itr++) = NULL;
    return lot;
}

void ffly_free_lot(ffly_lotp __lot) {
    __ffly_mem_free(__lot->top);
    __ffly_mem_free(__lot->objs);
}

void ffly_lot_cleanup() {
    ffly_lotp lot = top, prev = NULL;
    while(lot != NULL) {
        if (prev != NULL)
            __ffly_mem_free(prev);

        ffly_free_lot(lot);

        prev = lot;
        lot = lot->next;
    }

    if (end != NULL)
        __ffly_mem_free(end); 
}
