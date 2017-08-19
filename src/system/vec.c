# include "vec.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "io.h"
# include "../data/mem_ncpy.h"
# include "errno.h"
# include <errno.h>
# include <eint_t.h>
mdl_u8_t static is_flag(struct ffly_vec *__vec, mdl_u8_t __flag) {
	return __vec->flags & __flag == __flag?1:0;}

ffly_err_t ffly_vec_init(struct ffly_vec *__vec, mdl_uint_t __blk_size) {
	__vec->flags = 0;
	__vec->p = NULL;
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if ((__vec->p = __ffly_mem_alloc((++__vec->page_c)*(VEC_PAGE_SIZE*__blk_size))) == NULL) {
			ffly_printf(stderr, "vec: failed to alloc memory, errno: %d.\n", errno);
			return FFLY_FAILURE;
		}
	}

	__vec->off = 0;
	__vec->size = 0;
	__vec->blk_size = __blk_size;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_push_back(struct ffly_vec *__vec, void **__p) {
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if (__vec->off >> 5 > (__vec->off-1 >> 5)) {
			if ((__vec->p = ffly_mem_realloc(__vec->p, (++__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				ffly_printf(stderr, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
				return FFLY_FAILURE;
			}
		}
	}

	*__p = (void*)((mdl_u8_t*)__vec->p+(__vec->off*__vec->blk_size));
	__vec->off++;
	return FFLY_SUCCESS;
}

/*
	if theres a error with pop move ffly_byte_ncpy to the top
*/
ffly_err_t ffly_vec_pop_back(struct ffly_vec *__vec, void *__p) {
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if (__vec->off >> 5 < (__vec->off+1 >> 5) && __vec->page_c > 1) {
			if ((__vec->p = ffly_mem_realloc(__vec->p, (--__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				ffly_printf(stderr, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
				return FFLY_FAILURE;
			}
		}
	}

	// dont change - we cant return the ptr because if a resize takes place the pointer will be invalid
	ffly_mem_ncpy(__p, (void*)((mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size)), __vec->blk_size);
	__vec->off--;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_resize(struct ffly_vec *__vec, mdl_uint_t __size) {
	__vec->size = __size;
	if (!__size) {__ffly_mem_free(__vec->p); __vec->p = NULL; return FFLY_SUCCESS;}
	if (!__vec->p) {__vec->p = __ffly_mem_alloc(__size*__vec->blk_size); return FFLY_SUCCESS;}
	if ((__vec->p = ffly_mem_realloc(__vec->p, __size*__vec->blk_size)) == NULL) {
		ffly_printf(stderr, "vec: failed to realloc memory for resize, errno: %d\n", errno);
		return FFLY_FAILURE;}
	ffly_printf(stdout, "resize.\n");
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_de_init(struct ffly_vec *__vec) {__ffly_mem_free(__vec->p);return FFLY_SUCCESS;}
