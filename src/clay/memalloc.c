# include "memalloc.h"
# include "../malloc.h"

typedef struct hdr {
	struct hdr **bk, *next;
	void *p;
} *hdrp;

# define hdrsize sizeof(struct hdr)

static hdrp top = NULL;
void* clay_mem_alloc(ff_uint_t __c) {
	void *p = malloc(__c+hdrsize);
	hdrp h = (hdrp)p;
	h->bk = &top;
	h->next = top;
	if (top != NULL)
		top->bk = &h->next;
	top = h;
	return (h->p = (void*)((ff_u8_t*)p+hdrsize));
}

void clay_mem_free(void *__p) {
	hdrp h = (hdrp)((ff_u8_t*)__p-hdrsize);
	*h->bk = h->next;
	if (h->next != NULL)
		h->next->bk = h->bk;
	free(h);
}

void clay_mem_cleanup(void) {
	hdrp cur = top, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		clay_mem_free(bk->p);
	}
	top = NULL;
}
