# include "lot.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
static ffly_lotp top = NULL;
static ffly_lotp end = NULL;

void ffly_lot_add(ffly_lotp __lot, ffly_objp __obj) {
    mdl_uint_t xa_off = __obj->xa-__lot->xa;
    mdl_uint_t ya_off = __obj->ya-__lot->ya;
    mdl_uint_t za_off = __obj->za-__lot->za;
    *(*(__lot->objs+xa_off+(ya_off*__lot->yal)+(za_off*(__lot->yal*__lot->xal))) = __lot->end++) = __obj;
}

void ffly_lot_rm(ffly_lotp __lot, ffly_objp __obj) {
    mdl_uint_t xa_off = __obj->xa-__lot->xa;
    mdl_uint_t ya_off = __obj->ya-__lot->ya;
    mdl_uint_t za_off = __obj->za-__lot->za;
    ffly_objppp obj = __lot->objs+xa_off+(ya_off*__lot->yal)+(za_off*(__lot->yal*__lot->xal)); 
    if ((*obj)+1 == __lot->end)
        __lot->end--;
    else
        **obj = *(--__lot->end);
}

ffly_objpp ffly_lot_obj(ffly_lotp __lot, ffly_objp __obj) {
    mdl_uint_t xa_off = __obj->xa-__lot->xa;
    mdl_uint_t ya_off = __obj->ya-__lot->ya;
    mdl_uint_t za_off = __obj->za-__lot->za;
    return *(__lot->objs+xa_off+(ya_off*__lot->yal)+(za_off*(__lot->yal*__lot->xal)));
}

void ffly_lot_prepare(ffly_lotp __lot, mdl_uint_t __xa, mdl_uint_t __ya, mdl_uint_t __za) {
    __lot->xa = __xa;
    __lot->ya = __ya;
    __lot->za = __za;
}

ffly_lotp ffly_alloc_lot(mdl_uint_t __xal, mdl_uint_t __yal, mdl_uint_t __zal) {
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

    mdl_uint_t size = (lot->xal = __xal)*(lot->yal = __yal)*(lot->zal = __zal);
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
