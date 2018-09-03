# include "memalloc.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
typedef struct hdr {
	struct hdr **bk, *next;
	void *p;
} *hdrp;

# define hdrsize sizeof(struct hdr)

static hdrp top = NULL;

# define link(__h) \
	__h->bk = &top; \
	if (top != NULL) {  \
		top->bk = &__h->next;   \
	}               \
	__h->next = top;    \
	top = h;

# define delink(__h) \
	*__h->bk = __h->next;   \
	if (__h->next != NULL) {    \
		__h->next->bk = __h->bk; \
	}


void* ffly_tmalloc(ff_uint_t __size) {
	ff_u8_t *p;

	p = (ff_u8_t*)__ffly_mem_alloc(__size+hdrsize);

	hdrp h = (hdrp)p;
	link(h);
	return (h->p = (void*)(p+hdrsize));
}

void ffly_tfree(void *__p) {
	hdrp h = (hdrp)(((ff_u8_t*)__p)-hdrsize);

	delink(h);
	__ffly_mem_free(h);
}

void* ffly_trealloc(void *__p, ff_uint_t __size) {
	ff_u8_t *p;
	p = (ff_u8_t*)(((ff_u8_t*)__p)-hdrsize);

	hdrp h;

	h = (hdrp)p;
	delink(h);
	h = (hdrp)__ffly_mem_realloc(h, __size+hdrsize);
	link(h);

	return (h->p = (void*)(p+hdrsize));
}
// purge memory
void ffly_tmpurge(void) {
	hdrp cur, bk;
	cur = top;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		ffly_tfree(bk->p);
	}
}
