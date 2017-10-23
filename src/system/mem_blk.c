# include "mem_blk.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "io.h"
# include "errno.h"
ffly_err_t ffly_mem_blk_init(struct ffly_mem_blk *__mem_blk, mdl_uint_t __size, mdl_uint_t __slice_size) {
	if ((__mem_blk->p = (void*)__ffly_mem_alloc(__size*__slice_size)) == NULL) {
		ffly_printf(stderr, "failed to alloc memory.\n");
		return FFLY_FAILURE;
	}
	__mem_blk->off = 0;
	__mem_blk->size = __size;
	__mem_blk->slice_size = __slice_size;
	__mem_blk->uu_slices = NULL;
	__mem_blk->uu_slice_c = 0;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_mem_blk_de_init(struct ffly_mem_blk *__mem_blk) {
	__ffly_mem_free(__mem_blk->p);
	if (__mem_blk->uu_slices != NULL)
		__ffly_mem_free(__mem_blk->uu_slices);
	return FFLY_SUCCESS;
}

ffly_bool_t ffly_mem_blk_full(struct ffly_mem_blk *__mem_blk) {
	return ((__mem_blk->off > __mem_blk->size)&&!__mem_blk->uu_slice_c);
}

void* ffly_mem_blk_get(struct ffly_mem_blk *__mem_blk, mdl_uint_t __off) {
	return ((mdl_u8_t*)__mem_blk->p+(__off*__mem_blk->slice_size));
}

void* ffly_mem_blk_alloc(struct ffly_mem_blk *__mem_blk) {
	void *ret;
	if (__mem_blk->uu_slice_c > 0) {
		ret = *(void**)(__mem_blk->uu_slices+(__mem_blk->uu_slice_c-1));
		if (!(__mem_blk->uu_slice_c-1)) {
			__ffly_mem_free(__mem_blk->uu_slices);
			__mem_blk->uu_slices = NULL;
			__mem_blk->uu_slice_c--;
		} else {
			if ((__mem_blk->uu_slices = (void*)__ffly_mem_realloc(__mem_blk->uu_slices, ((__mem_blk->uu_slice_c--)*sizeof(void*)))) == NULL) {

			}
		}
		return ret;
	}

	ret = (void*)((mdl_u8_t*)__mem_blk->p+((__mem_blk->off++)*__mem_blk->slice_size));
	return ret;
}

void ffly_mem_blk_free(struct ffly_mem_blk *__mem_blk, void *__p) {
	if (!__mem_blk->uu_slice_c) {
		__mem_blk->uu_slices = (void*)__ffly_mem_alloc(sizeof(void*));
		__mem_blk->uu_slice_c++;
	} else
		__mem_blk->uu_slices = (void*)__ffly_mem_realloc(__mem_blk->uu_slices, (++__mem_blk->uu_slice_c)*sizeof(void*));
	*(void**)(__mem_blk->uu_slices+(__mem_blk->uu_slice_c-1)) = __p;
}
