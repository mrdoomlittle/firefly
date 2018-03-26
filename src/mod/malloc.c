# include "malloc.h"
# include "../ffly_def.h"
# include "ring.h"
# include "../call.h"
/*
	keep small
*/

mdl_u8_t static opt = FF_MAL_O_LOC;
mdl_u8_t static heap[8048];
mdl_u8_t static *fresh = heap;

typedef struct hdr {
	struct hdr *prev, *next, *fd, *bk;
	mdl_u16_t size;
	mdl_u8_t inuse, *end;
} *hdrp;

# define hdr_size sizeof(struct hdr)

hdrp static top = NULL, bin = NULL;

# define is_opt(__opt) \
	(__opt == opt)

void setmalopt(mdl_u8_t __opt) {
	opt = __opt;
}

# include "printf.h"
void pr() {
	printf("---* all *---\n");
	struct hdr buf[128];
	hdrp bufp = buf;

	hdrp cur = top;
	while(cur != NULL) {
		*(bufp++) = *cur;
		cur = cur->next;
	}

	cur = buf;
	while(cur != bufp) {
		printf("size: %u, inuse: %u\n", cur->size, cur->inuse);
		cur++;
	}
}

void pf() {
	printf("---* free *---\n");
	struct hdr buf[128];
	hdrp bufp = buf;

	hdrp cur = bin;
	while(cur != NULL) {	
		*(bufp++) = *cur;
		cur = cur->fd;
	}

	cur = buf;
	while(cur != bufp) {
		printf("size: %u, inuse: %u\n", cur->size, cur->inuse);
		cur++;
	}
}

void detach(hdrp __slab) {
	if (__slab == top) {
		if ((top = top->next) != NULL)
			top->prev = NULL;
	} else {
		if (__slab->prev != NULL)
 			__slab->prev->next = __slab->next;
		if (__slab->next != NULL)
			__slab->next->prev = __slab->prev;
	}
}

void unlink(hdrp __slab) {
	if (__slab == bin) {
		if ((bin = bin->fd) != NULL)
			bin->bk = NULL;
	} else {
		if (__slab->fd != NULL)
			__slab->fd->bk = __slab->bk;
		if (__slab->bk != NULL)
			__slab->bk->fd = __slab->fd;
	}
	__slab->fd = NULL;
	__slab->bk = NULL;
}


void* malloc(mdl_uint_t __bc) {
	if (is_opt(FF_MAL_O_LOC)) {
		if (bin != NULL) {
			hdrp cur = bin;
			while(cur != NULL) {
				if (cur->size >= __bc) {
					cur->inuse = 1;
					unlink(cur);
					if (cur->size>__bc+hdr_size) {
						hdrp h = (hdrp)((mdl_u8_t*)cur+(__bc+hdr_size));
						*h = (struct hdr) {
							.prev=cur, .next=cur->next,
							.fd=NULL, .bk=NULL, .size=cur->size-(hdr_size+__bc),
							.inuse=1,.end=cur->end
						};

						cur->next = h;
						cur->size = __bc;
						cur->end = (mdl_u8_t*)h;
						if (h->next != NULL)
							h->next->prev = h;

						free((mdl_u8_t*)h+hdr_size);
					}

					return (void*)((mdl_u8_t*)cur+hdr_size);
				}
				cur = cur->fd;
			}
		}

		mdl_u8_t *p = fresh;
		fresh+=hdr_size+__bc;

		hdrp h = (hdrp)p;
		h->fd = NULL;
		h->bk = NULL;
		h->inuse = 1;
		h->next = top;
		if (top != NULL)
			top->prev = h;
		h->prev = NULL;
		top = h;
		h->size = __bc;
		h->end = fresh;
		return (void*)(p+hdr_size);
	}

	void *ret;
	ffmod_ring(_ffcal_malloc, &ret, &__bc);
	return ret;
}

void free(void *__p) {
	if (is_opt(FF_MAL_O_LOC)) {
		hdrp h = (hdrp)((mdl_u8_t*)__p-hdr_size);

		hdrp prev = h->prev, next;
		if (prev != NULL) {
			if (!prev->inuse) {
				detach(h);
				unlink(prev);
				prev->size+=hdr_size+h->size;
				prev->end = h->end;
				h = prev;
			}
		}

		next = h->next;
		if (next != NULL) {
			if (!next->inuse) {
				detach(next);
				unlink(next);
				h->end = next->end;
				h->size+=hdr_size+next->size;
			}
		}

		if (fresh == h->end) {
			detach(h);
			fresh = (mdl_u8_t*)h;
			return;
		}

		h->inuse = 0;
		h->fd = bin;
		if (bin != NULL)
			bin->bk = h;
		h->bk = NULL;
		bin = h;
		return;
	}

	ffmod_ring(_ffcal_free, NULL, &__p);
}

void* realloc(void *__p, mdl_uint_t __bc) {
	if (is_opt(FF_MAL_O_LOC)) {


		return NULL;
	}

	mdl_u8_t p[sizeof(void*)+sizeof(mdl_uint_t)];
	
	*(void**)p = __p;
	*(mdl_uint_t*)(p+sizeof(void*)) = __bc;

	void *ret;
	ffmod_ring(_ffcal_realloc, &ret, p);
	return ret;
}
