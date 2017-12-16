# include "arr.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_realloc.h"
# include "../memory/mem_free.h"
# include "io.h"
# include "../data/mem_cpy.h"
# include "err.h"
ffly_err_t ffly_arr_init(struct ffly_arr *__arr, mdl_uint_t __blk_size, mdl_uint_t __blk_c) {
	if ((__arr->p = __ffly_mem_alloc(__blk_c*__blk_size)) == NULL) {
		ffly_fprintf(ffly_err, "arr: failed to allocate memory.\n");
		return FFLY_FAILURE;
	}

	__arr->blk_size = __blk_size;
	__arr->blk_c = __blk_c;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_arr_resize(struct ffly_arr *__arr, mdl_uint_t __blk_c) {
	if ((__arr->p = __ffly_mem_realloc(__arr->p, (__arr->blk_c = __blk_c)*__arr->blk_size)) == NULL) {
		ffly_fprintf(ffly_err, "arr: failed to reallocate memory.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

void* ffly_arr_get(struct ffly_arr *__arr, ffly_off_t __blk_off) {
	return (void*)((mdl_u8_t*)__arr->p+(__blk_off*__arr->blk_size));}

void ffly_arr_put(struct ffly_arr *__arr, void *__p, ffly_off_t __blk_off) {
	ffly_mem_cpy((void*)((mdl_u8_t*)__arr->p+(__blk_off*__arr->blk_size)), __p, __arr->blk_size);}

ffly_err_t ffly_arr_de_init(struct ffly_arr *__arr) {
	if (__arr->p != NULL) {
		if (_err(__ffly_mem_free(__arr->p))) {
			ffly_fprintf(ffly_err, "arr: failed to free.\n");
			return FFLY_FAILURE;
		}
	}
	return FFLY_SUCCESS;
}
