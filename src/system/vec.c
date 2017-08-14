# include "vec.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "io.h"
# include "../data/byte_ncpy.h"
# include "errno.h"
# include <errno.h>
# include <eint_t.h>
mdl_u8_t static is_flag(struct vec *__vec, mdl_u8_t __flag) {
	return __vec->flags & __flag == __flag?1:0;}

ffly_err_t ffly_vec_init(struct vec *__vec, mdl_uint_t __blk_size) {
	__vec->flags = 0;
	__vec->p = NULL;
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if ((__vec->p = __ffly_mem_alloc((++__vec->page_c)*(VEC_PAGE_SIZE*__blk_size))) == NULL) {
			ffly_printf(stderr, "vec: failed to alloc memory, errno: %d.\n", errno);
			return FFLY_FAILURE;
		}
	}

	__vec->off = 0;
	__vec->blk_size = __blk_size;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_push_back(struct vec *__vec, void **__ptr) {
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if (__vec->off >> 5 > (__vec->off-1 >> 5)) {
			if ((__vec->p = ffly_mem_realloc(__vec->p, (++__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				ffly_printf(stderr, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
				return FFLY_FAILURE;
			}
		}
	}

	*__ptr = (void*)((mdl_u8_t*)__vec->p+(__vec->off*__vec->blk_size));
	__vec->off++;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_pop_back(struct vec *__vec, void *__ptr) {
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if (__vec->off >> 5 < (__vec->off+1 >> 5) && __vec->page_c > 1) {
			if ((__vec->p = ffly_mem_realloc(__vec->p, (--__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				ffly_printf(stderr, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
				return FFLY_FAILURE;
			}
		}
	}

	ffly_byte_ncpy(__ptr, (void*)((mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size)), __vec->blk_size);
	__vec->off--;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_resize(struct vec *__vec, mdl_uint_t __size) {
	if (!__size) {__ffly_mem_free(__vec->p); __vec->p = NULL; return FFLY_SUCCESS;}
	if (!__vec->p) {__vec->p = __ffly_mem_alloc(__size*__vec->blk_size); return FFLY_SUCCESS;}
	if ((__vec->p = ffly_mem_realloc(__vec->p, __size*__vec->blk_size)) == NULL) {
		ffly_printf(stderr, "vec: failed to realloc memory for resize, errno: %d\n", errno);
		return FFLY_FAILURE;}
	ffly_printf(stdout, "resize.\n");
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_de_init(struct vec *__vec) {__ffly_mem_free(__vec->p);return FFLY_SUCCESS;}
