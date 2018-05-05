# include "plate.h"
# include "mem_alloc.h"
# include "mem_free.h"
# include "mem_realloc.h"
// ignore for now

# define PAGE_SHIFT 4
# define PAGE_SIZE (1<<PAGE_SHIFT)

typedef struct plate {
	ff_uint_t no;
	void *p;
	struct plate *link;
} *platep;

# define plate_size sizeof(struct plate)

platep *plates = NULL;
platep free = NULL;
ff_uint_t static page_c = 0;
ff_uint_t static off = 0;

void ffly_plate_store(void *__plate, void **__p) {
	*__p = ((platep)__plate)->p;
}

void ffly_plate_load(void *__plate, void *__p) {
	((platep)__plate)->p = __p;
}

ff_uint_t ffly_plateno(void *__plate) {
	return ((platep)__plate)->no;
}

void* ffly_plate_at(ff_uint_t __no) {
	ff_uint_t page = __no>>PAGE_SHIFT;
	return *(plates+page)+(__no-(page*PAGE_SIZE));
}

void* ffly_plate_alloc(void) {
	if (free != NULL) {
		platep ret = free;
		free = free->link;
		return ret;
	}

	platep p;
	ff_uint_t page = off>>PAGE_SHIFT;
	
	if (!plates) {
		plates = (platep*)__ffly_mem_alloc(sizeof(platep));
		page_c++;
	} else {
		if (page>page_c-1) {
			plates = (platep*)__ffly_mem_realloc(plates, (++page_c)*sizeof(platep));	
		} else
			goto _sk;
	}

	*(plates+page) = (platep)__ffly_mem_alloc(PAGE_SIZE*plate_size);
_sk:
	p = (*(plates+page))+(off-(page*PAGE_SIZE));
	p->no = off++;
	return p;
}

void ffly_plate_free(void *__plate) {
	platep p = (platep)__plate;
	if (p->no == off-1) {
		off--;
		ff_uint_t page = off>>PAGE_SHIFT;
		if (page < page_c-1 && page_c>1) {
			__ffly_mem_free(*(plates+(page+1)));
			plates = (platep*)__ffly_mem_realloc(plates, (--page_c)*sizeof(platep));
		}
	} else  {
		p->link = free;
		free = p;
	}
}

void ffly_plate_cleanup() {
	platep *cur = plates;
	platep *end = cur+page_c;
	while(cur != end)
		__ffly_mem_free(*(cur++));
	__ffly_mem_free(plates);
}
