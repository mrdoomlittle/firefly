# include "malloc.h"
# include "../ffly_def.h"
# include "ring.h"
# include "../call.h"
/*
	keep small
*/

mdl_u8_t static opt = FF_MAL_O_LOC;
mdl_u8_t static heep[2048];
mdl_u8_t static *fresh = heep;

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

void* malloc(mdl_uint_t __bc) {
	if (is_opt(FF_MAL_O_LOC)) {
		if (bin != NULL) {
			hdrp cur = bin;
			while(cur != NULL) {
				if (cur->size >= __bc && !cur->inuse) {
					cur->inuse = 1;
					if (cur == bin) {
						if ((bin = bin->fd) != NULL)
							bin->bk = NULL;
					} else {
						if (cur->fd != NULL)
							cur->fd->bk = cur->bk;
						if (cur->bk != NULL)
							cur->bk->fd = cur->fd;
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
		if (fresh == h->end) {
			if (h == top) {
				if ((top = top->next) != NULL)
					top->prev = NULL;
			} else {
				if (h->prev != NULL)
					h->prev->next = h->next;
				if (h->next != NULL)
					h->next->prev = h->prev;
			}
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
