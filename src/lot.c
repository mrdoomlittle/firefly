# include "lot.h"
# include "system/io.h"
# include "system/errno.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
static ffly_lotp top = NULL;
ff_mlock_t lock = FFLY_MUTEX_INIT;
# define lk(__lot) \
	ffly_mutex_lock(&(__lot)->lock)
# define ul(__lot) \
	ffly_mutex_unlock(&(__lot)->lock)

void ffly_lot_add(ffly_lotp __lot, ffly_phy_bodyp __body) {
	if (!__body || !__lot) {
		return;
	}

	if (__lot->end>=__lot->top+__lot->size) {
		ffly_fprintf(ffly_err, "failure adding body to lot.\n");
		return;
	}

	ff_uint_t xoff = (*__body->x)-__lot->x;
	ff_uint_t yoff = (*__body->y)-__lot->y;
	ff_uint_t zoff = (*__body->z)-__lot->z;
	ffly_phy_bodyppp body = __lot->bodies+xoff+(yoff*__lot->xl)+(zoff*(__lot->yl*__lot->xl));
	if (body>=__lot->bodies+__lot->size) {
		ffly_fprintf(ffly_err, "lot, body is out of bounds, {%u:%u:%u}, {%u:%u:%u}, {%u:%u:%u}, body: %u\n", xoff, yoff, zoff, __lot->x, __lot->y, __lot->z, *__body->x, *__body->y, *__body->z, __body->id);
		goto _fail;
	}
	
	if (*body != NULL) {
		ffly_printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@lot, unclean memory, %u:%u:%u\n", xoff, yoff, zoff);
		goto _fail;
	}

	ffly_printf("lot usage: %u:%u\n", __lot->size, __lot->end-__lot->top);
	*(*(__body->p = body) = __lot->end++) = __body;
	ffly_printf("body: %u added to lot %p\n", __body->id, __lot);
	return;
_fail:
	ffly_fprintf(ffly_err, "failed to add to lot.\n");
}

void ffly_lot_rm(ffly_lotp __lot, ffly_phy_bodyp __body) {
	if (!__lot || !__body) {
		ffly_fprintf(ffly_err, "failed to remove from lot.\n");
		return;
	}

	__body->lot = NULL;

	ff_uint_t xoff = (*__body->x)-__lot->x;
	ff_uint_t yoff = (*__body->y)-__lot->y;
	ff_uint_t zoff = (*__body->z)-__lot->z;
	ffly_phy_bodyppp body = __lot->bodies+xoff+(yoff*__lot->xl)+(zoff*(__lot->yl*__lot->xl)); 
	if (body>=__lot->bodies+__lot->size) {
		ffly_fprintf(ffly_err, "lot, body is out of bounds, {%u:%u:%u}, {%u:%u:%u}, {%u:%u:%u}, body: %u\n", xoff, yoff, zoff, __lot->x, __lot->y, __lot->z, *__body->x, *__body->y, *__body->z, __body->id);
		goto _fail;
	}

	if (!*body) {
		ffly_fprintf(ffly_err, "lot, body is not apart of lot.\n");
		goto _fail;
	}

	if (!**body) {
		ffly_fprintf(ffly_err, "lot, body at location does not exist.\n");
		goto _fail;
	}

	if (**body != __body) {
		ffly_fprintf(ffly_err, "body alreay at this location.\n");
		goto _fail;
	}

	if (__body->p != body) {
		ffly_fprintf(ffly_err, "body location does not match.\n");
		goto _fail;
	}

	if (*body == __lot->end-1) 
		__lot->end--;
	else { 
		ffly_phy_bodyp t = *(--__lot->end);
		*(*t->p = *body) = t;
	}
	*body = NULL;
	return;
_fail:
	ffly_fprintf(ffly_err, "failed to remove from lot.\n");
}

ffly_phy_bodypp ffly_lot_obj(ffly_lotp __lot, ffly_phy_bodyp __body) {
	ff_uint_t xoff = *__body->x-__lot->x;
	ff_uint_t yoff = *__body->y-__lot->y;
	ff_uint_t zoff = *__body->z-__lot->z;
	return *(__lot->bodies+xoff+(yoff*__lot->xl)+(zoff*(__lot->yl*__lot->xl)));
}

void ffly_lot_prepare(ffly_lotp __lot, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	__lot->x = __x;
	__lot->y = __y;
	__lot->z = __z;
}

ffly_lotp ffly_lot_top(void) {
	return top;
}

ffly_lotp ffly_lot_alloc(ff_uint_t __xl, ff_uint_t __yl, ff_uint_t __zl) {
	ffly_lotp lot = (ffly_lotp)__ffly_mem_alloc(sizeof(struct ffly_lot));
	lot->lock = FFLY_MUTEX_INIT;
	ffly_mutex_lock(&lock);
	lk(lot);
	if (top != NULL) {
		lk(top);
		top->bk = &lot->next;
		top->prev = lot;
	}
	lot->next = top;
	lot->bk = &top;
	lot->prev = NULL;
	top = lot;
	if (lot->next != NULL)
		ul(lot->next);
	ul(lot);
	ffly_mutex_unlock(&lock);

	ff_uint_t size = (lot->xl = __xl)*(lot->yl = __yl)*(lot->zl = __zl);
	lot->size = size;
	lot->top = (ffly_phy_bodypp)__ffly_mem_alloc(size*sizeof(ffly_phy_bodyp));
	lot->end = lot->top;

	lot->bodies = (ffly_phy_bodyppp)__ffly_mem_alloc(size*sizeof(ffly_phy_bodypp));
	ffly_phy_bodyppp itr = lot->bodies;
	while(itr != lot->bodies+size)
		*(itr++) = NULL;
	ffly_printf("new lot alloc %p\n", lot);
	return lot;
}

void ffly_lot_free(ffly_lotp __lot) {
	lk(__lot);
	ffly_lotp next;
	if ((next = __lot->next) != NULL) {
		lk(next);
		next->prev = __lot->prev;
		next->bk = __lot->bk;
	}

	if ((*__lot->bk = next) != NULL)
		ul(next);
	ul(__lot);
	__ffly_mem_free(__lot->top);
	__ffly_mem_free(__lot->bodies);
	__ffly_mem_free(__lot);
}

ffly_lotp ffly_lot_creat(ff_uint_t __x, ff_uint_t __y, ff_uint_t __z, ff_uint_t __xl, ff_uint_t __yl, ff_uint_t __zl) {
	ffly_lotp lot;
	lot = ffly_lot_alloc(__xl, __yl, __zl);
	ffly_lot_prepare(lot, __x, __y, __z);
	return lot;
}

void ffly_lot_cleanup(void) {
	ffly_lotp cur = top, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		ffly_lot_free(bk);
	}
}
