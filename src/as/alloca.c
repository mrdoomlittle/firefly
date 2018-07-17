# include "as.h"
# include "../malloc.h"
typedef struct hdr {
	struct hdr *next, **bk;
} *hdrp;


hdrp static top = NULL;
# define hdrsize sizeof(struct hdr)

void ff_as_al_cu() {
	if (!top) return;
	hdrp cur = top, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		free(bk);
	}
}
# define link(__h) \
	__h->bk = &top;	\
	if (top != NULL) {	\
		top->bk = &__h->next;	\
	}				\
	__h->next = top;	\
	top = h;

void *ff_as_al(ff_uint_t __size) {
	ff_u8_t *p = (ff_u8_t*)malloc(hdrsize+__size);
	hdrp h = (hdrp)p;
	link(h);
	return p+hdrsize;
}

# define delink(__h) \
	*__h->bk = __h->next;	\
	if (__h->next != NULL) {	\
		__h->next->bk = __h->bk; \
	}

void ff_as_fr(void *__p) {
	hdrp h = (ff_u8_t*)__p-hdrsize;
	delink(h);
	free(h);
}

void *ff_as_ral(void *__p, ff_uint_t __size) {
	hdrp h = (ff_u8_t*)__p-hdrsize;
	delink(h);
	
	h = (hdrp)realloc(h, hdrsize+__size);
	link(h);
	return (ff_u8_t*)h+hdrsize;
}
