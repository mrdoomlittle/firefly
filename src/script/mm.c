# include "mm.h"
# include "../ffly_def.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
typedef struct hdr {
	struct hdr *prev, *next;
} *hdrp;

# define hdrsize sizeof(struct hdr)
struct hdr *top = NULL;
void* ffs_mmap(ff_uint_t __size) {
	void *p = __ffly_mem_alloc(hdrsize+__size);
	hdrp h = (hdrp)p;
	h->next = top;
	h->prev = NULL;
	top = h;
	return (void*)((ff_u8_t*)p+hdrsize);
}

void ffs_munmap(void *__p) {
	hdrp h = (hdrp)((ff_u8_t*)__p-hdrsize);
	if (h == top) {
		if ((top = h->next) != NULL)
			top->prev = NULL;
	} else {
		if (h->prev != NULL)
			h->prev->next = h->next;
		if (h->next != NULL)
			h->next->prev = h->prev;
	}
	__ffly_mem_free(h);
}
