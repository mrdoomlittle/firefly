# include "memalloc.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../dep/mem_set.h"
# define PAGE_SHIFT 4
# define PAGE_SIZE (1<<PAGE_SHIFT)

ff_uint_t static page_c = 0;
ff_u32_t static off = 0;

void static ***space = NULL;
void* ff_hs_mem_alloc(ff_uint_t __sz) {
	ff_uint_t pg;
	ff_uint_t pg_off;

	void **slit, *p;

	pg = off>>PAGE_SHIFT;
	pg_off = off-(pg*PAGE_SIZE);
	off++;
	if (!space) {
		space = (void***)__ffly_mem_alloc(sizeof(void**));
		page_c++;
	} else {
		if (pg>=page_c) {
			space = (void***)__ffly_mem_realloc(space, (++page_c)*sizeof(void**));
		} else
			goto _sk;
	}

	/*
		for mem clean
	*/
	ffly_mem_set(*(space+pg) = (void**)__ffly_mem_alloc(PAGE_SIZE*sizeof(void*)), 0, PAGE_SIZE*sizeof(void*));
_sk:
	slit = (*(space+pg))+pg_off;
	*slit = (p = __ffly_mem_alloc(sizeof(void**)+__sz));
	*(void***)p = slit;
	p = (ff_u8_t*)p+sizeof(void**);
	return p;
}

void* ff_hs_mem_realloc(void *__p, ff_uint_t __sz) {
	void **slit;
	slit = *(void***)((ff_u8_t*)__p-sizeof(void**));
	slit = __ffly_mem_realloc(slit, sizeof(void**)+__sz);
	return ((ff_u8_t*)(*slit = slit))+sizeof(void**);
}

// might later
void ff_hs_mem_free(void *__p) {
}

void ff_hs_mem_clean(void) {
	void ***pg, ***end;
	void **p;
	pg = space;

	ff_uint_t i, page;
	end = pg+(page = (off>>PAGE_SHIFT));
	while(pg != end) {
		i = 0;
		p = *(pg++);
		while(i != PAGE_SIZE) {
			__ffly_mem_free(*(p++));
			i++;
		}
		__ffly_mem_free(*(pg-1));
	}	

	if (page != page_c) {
		i = 0;
		p = *pg;
		for(;i != PAGE_SIZE;i++) {
			if (*p != NULL)
				__ffly_mem_free(*p);
			p++;
		}
		__ffly_mem_free(*pg);
	}

	page_c = 0;
	off = 0;
	__ffly_mem_free(space);
}
