# include "lot.h"
# include "system/io.h"
# include "system/errno.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
static ffly_lotp top = NULL;
static ffly_lotp end = NULL;

void ffly_lot_add(ffly_lotp __lot, ffly_phy_bodyp __body) {
	if (!__body) {
		return;
	}

	if (__lot->end>=__lot->top+__lot->size) {
		ffly_fprintf(ffly_err, "failure adding body to lot.\n");
		return;
	}
	ff_uint_t xoff = *__body->x-__lot->x;
	ff_uint_t yoff = *__body->y-__lot->y;
	ff_uint_t zoff = *__body->z-__lot->z;
	ffly_phy_bodyppp body = __lot->bodies+xoff+(yoff*__lot->xl)+(zoff*(__lot->yl*__lot->xl));
	if (body>=__lot->bodies+__lot->size) {
		ffly_fprintf(ffly_err, "body is out of bounds.\n");
		return;
	}

	ffly_printf("lot usage: %u:%u\n", __lot->size, __lot->end-__lot->top);
	*(*(__body->p = body) = __lot->end++) = __body;
}

void ffly_lot_rm(ffly_lotp __lot, ffly_phy_bodyp __body) {
	if (!__lot || !__body) {
		ffly_fprintf(ffly_err, "failed to remove from lot.\n");
		return;
	}

	ff_uint_t xoff = *__body->x-__lot->x;
	ff_uint_t yoff = *__body->y-__lot->y;
	ff_uint_t zoff = *__body->z-__lot->z;
	ffly_phy_bodyppp body = __lot->bodies+xoff+(yoff*__lot->xl)+(zoff*(__lot->yl*__lot->xl)); 
	if (body>=__lot->bodies+__lot->size) {
		ffly_fprintf(ffly_err, "body is out of bounds.\n");
		return;
	}

	if (!*body) {
		ffly_fprintf(ffly_err, "body is not apart of lot.\n");
		return;
	}

	if (!**body) {
		ffly_fprintf(ffly_err, "body at location does not exist.\n");
		return;
	}

	if (*body == __lot->end-1) {
		__lot->end--;
		*body = NULL;
	} else { 
		ffly_phy_bodyp t = *(--__lot->end);
		*t->p = NULL;
		*t->p = *body;
	}	
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

ffly_lotp ffly_lot_alloc(ff_uint_t __xl, ff_uint_t __yl, ff_uint_t __zl) {
	ffly_lotp lot = (ffly_lotp)__ffly_mem_alloc(sizeof(struct ffly_lot));
	if (!top)
		top = lot;

	lot->prev = end;
	lot->next = NULL;
	if (end != NULL)
		end->next = lot;
	end = lot;

	ff_uint_t size = (lot->xl = __xl)*(lot->yl = __yl)*(lot->zl = __zl);
	lot->size = size;
	lot->top = (ffly_phy_bodypp)__ffly_mem_alloc(size*sizeof(ffly_phy_bodyp));
	lot->end = lot->top;

	lot->bodies = (ffly_phy_bodyppp)__ffly_mem_alloc(size*sizeof(ffly_phy_bodypp));
	ffly_phy_bodyppp itr = lot->bodies;
	while(itr != lot->bodies+size)
		*(itr++) = NULL;
	return lot;
}

void ffly_lot_free(ffly_lotp __lot) {
	__ffly_mem_free(__lot->top);
	__ffly_mem_free(__lot->bodies);
}

void ffly_lot_cleanup() {
	ffly_lotp cur = top, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		ffly_lot_free(bk);
		__ffly_mem_free(bk);
	}
}
