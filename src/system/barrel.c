# include "barrel.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
void ffly_barrel_init(ffly_barrelp __barrel, ff_uint_t __blksize) {
	__barrel->blksize = __blksize;
	__barrel->blocks = (void**)__ffly_mem_alloc(FFLY_BARREL_SIZE*sizeof(void*));
	__barrel->unused = (void**)__ffly_mem_alloc(FFLY_BARREL_SIZE*sizeof(void*));

	__barrel->fresh = __barrel->blocks;
	__barrel->next = __barrel->unused;

	void **block = __barrel->blocks;
	while(block != __barrel->blocks+FFLY_BARREL_SIZE)
		*(block++) = NULL;
	__barrel->link = NULL;
}

ff_bool_t ffly_barrel_full(ffly_barrelp __barrel) {
	return (__barrel->fresh == __barrel->blocks+FFLY_BARREL_SIZE
			&& __barrel->next == __barrel->unused);
}

void ffly_barrel_de_init(ffly_barrelp __barrel) {
	void **block = __barrel->blocks;
	while(block != __barrel->fresh) {
		if (*block != NULL)
			__ffly_mem_free(*block);
		block++;
	}

	__ffly_mem_free(__barrel->blocks);
	__ffly_mem_free(__barrel->unused);
}

void* ffly_barrel_alloc(ffly_barrelp __barrel) {
	void **block;
	if (__barrel->next>__barrel->unused)
		block = *(--__barrel->next);
	else {
		block = __barrel->fresh++;
		*block = __ffly_mem_alloc(__barrel->blksize);
	}
	return *block;
}

void ffly_barrel_free(ffly_barrelp __barrel, void *__p) {
	if (__p = *(__barrel->fresh-1)) {
		*(--__barrel->fresh) = NULL;
		__ffly_mem_free(__p);
	} else
		*(__barrel->next++) = __p;	
}
