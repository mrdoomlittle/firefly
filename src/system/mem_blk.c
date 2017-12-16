# include "mem_blk.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "io.h"
# include "errno.h"
# include "mutex.h"
# include "err.h"
ffly_err_t ffly_mem_blk_init(struct ffly_mem_blk *__mem_blk, ffly_size_t __size, ffly_size_t __slice_size) {
	if ((__mem_blk->p = (void*)__ffly_mem_alloc(__size*__slice_size)) == NULL) {
		ffly_fprintf(ffly_err, "mem_blk: failed to allocate memory.\n");
		return FFLY_FAILURE;
	}
	__mem_blk->m = FFLY_MUTEX_INIT;
	__mem_blk->off = 0;
	__mem_blk->size = __size;
	__mem_blk->slice_size = __slice_size;
	__mem_blk->uu_slices = NULL;
	__mem_blk->uu_slice_c = 0;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_mem_blk_de_init(struct ffly_mem_blk *__mem_blk) {
	ffly_err_t err;
	if (_err(err = __ffly_mem_free(__mem_blk->p))) {
		ffly_fprintf(ffly_err, "mem_blk: failed to free.\n");
		return err;
	}

	if (__mem_blk->uu_slices != NULL) {
		if (_err(err = __ffly_mem_free(__mem_blk->uu_slices))) {
			ffly_fprintf(ffly_err, "mem_blk: failed to free.");
			return err;
		}
	}
	return FFLY_SUCCESS;
}

ffly_bool_t ffly_mem_blk_full(struct ffly_mem_blk *__mem_blk) {
	return ((__mem_blk->off == __mem_blk->size-1)&&!__mem_blk->uu_slice_c);
}

void* ffly_mem_blk_get(struct ffly_mem_blk *__mem_blk, mdl_uint_t __off) {
	return ((mdl_u8_t*)__mem_blk->p+(__off*__mem_blk->slice_size));
}

void* ffly_mem_blk_alloc(struct ffly_mem_blk *__mem_blk) {
	if (!__mem_blk->uu_slice_c && __mem_blk->off == __mem_blk->size-1) return NULL;
	void *ret;
	ffly_mutex_lock(&__mem_blk->m);
	if (__mem_blk->uu_slice_c > 0) {
		ret = *(void**)((mdl_u8_t*)__mem_blk->uu_slices+((__mem_blk->uu_slice_c-1)*sizeof(void*)));
		if (!(__mem_blk->uu_slice_c-1)) {
			if (_err(__ffly_mem_free(__mem_blk->uu_slices))) {
				ffly_fprintf(ffly_err, "mem_blk: failed to free.\n");
				goto _fatal;
			}
			__mem_blk->uu_slices = NULL;
			__mem_blk->uu_slice_c--;
		} else {
			if ((__mem_blk->uu_slices = (void*)__ffly_mem_realloc(__mem_blk->uu_slices, ((--__mem_blk->uu_slice_c)*sizeof(void*)))) == NULL) {
				ffly_fprintf(ffly_err, "mem_blk: failed to reallocate memory.\n");
				goto _fatal;
			}
		}
		ffly_mutex_unlock(&__mem_blk->m);
		return ret;
	}
	ret = (void*)((mdl_u8_t*)__mem_blk->p+((__mem_blk->off++)*__mem_blk->slice_size));
	ffly_mutex_unlock(&__mem_blk->m);
	return ret;
	_fatal:
	ffly_mutex_unlock(&__mem_blk->m);
	return NULL;
}

ffly_err_t ffly_mem_blk_free(struct ffly_mem_blk *__mem_blk, void *__p) {
	ffly_err_t err = FFLY_SUCCESS;
	ffly_mutex_lock(&__mem_blk->m);
	if (!__mem_blk->uu_slice_c) {
		if ((__mem_blk->uu_slices = (void*)__ffly_mem_alloc(sizeof(void*))) == NULL) {
			ffly_fprintf(ffly_err, "mem_blk: failed to allocate memory.\n");
			err = FFLY_FAILURE;
			goto _end;
		}
		__mem_blk->uu_slice_c++;
	} else {
		if ((__mem_blk->uu_slices = (void*)__ffly_mem_realloc(__mem_blk->uu_slices, (++__mem_blk->uu_slice_c)*sizeof(void*))) == NULL) {
			ffly_fprintf(ffly_err, "mem_blk: failed to reallocate memory.\n");
			err = FFLY_FAILURE;
			goto _end;
		}
	}
	*(void**)((mdl_u8_t*)__mem_blk->uu_slices+((__mem_blk->uu_slice_c-1)*sizeof(void*))) = __p;
	_end:
	ffly_mutex_unlock(&__mem_blk->m);
	return err;
}
