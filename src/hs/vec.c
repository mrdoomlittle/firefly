# include "vec.h"
# include "memalloc.h"
# include "../ffly_def.h"
# define PAGE_SHIFT 4
# define PAGE_SIZE (1<<PAGE_SHIFT)
void ff_hs_vec_init(ff_hs_vecp __vec, ff_u16_t __blksize) {
	__vec->off = 0;
	__vec->blksize = __blksize;
	__vec->p = NULL;
	__vec->page_c = 0;
}

void* ff_hs_vec_push(ff_hs_vecp __vec) {
	void *r;
	ff_uint_t pg;

	pg = __vec->off>>PAGE_SHIFT;
	if (!__vec->p) {
		__vec->p = ff_hs_mem_alloc(PAGE_SIZE*__vec->blksize);
		__vec->page_c++;
	} else {
		if (pg>=__vec->page_c)
			__vec->p = ff_hs_mem_realloc(__vec->p, (++__vec->page_c)*(PAGE_SIZE*__vec->blksize));
	}
	r = (ff_u8_t*)__vec->p+(__vec->off++*__vec->blksize);
	return r;
}
