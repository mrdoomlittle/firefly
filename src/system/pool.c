# include "pool.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "io.h"
# include "errno.h"
# include "mutex.h"
# include "err.h"
# include "../oddity.h"
/*
	move to memory/pool.h
*/

#ifndef FF_POOL_SA
#define mem_alloc(__n)\
	__ffly_mem_alloc(__n)	
#define mem_free(__p)\
	__ffly_mem_free(__p)
#define mem_realloc(__p, __n)\
	__ffly_mem_realloc(__p, __n)
#else
#define mem_alloc(__n)\
	__pool->alloc(__pool->arg, __n)
#define mem_free(__p)\
	__pool->free(__pool->arg, __p)
#define mem_realloc(__p, __n)\
	__pool->realloc(__pool->arg, __p, __n)
#endif
ff_err_t ffly_pool_init(struct ffly_pool *__pool, ff_size_t __size, ff_size_t __slice_size) {
	if ((__pool->p = mem_alloc(__size*__slice_size)) == NULL) {
		ffly_fprintf(ffly_err, "pool: failed to allocate memory.\n");
		caught_oddity;
		return FFLY_FAILURE;
	}
	__pool->lock = FFLY_MUTEX_INIT;
	__pool->off = 0;
	__pool->size = __size;
	__pool->slice_size = __slice_size;
	__pool->uu_slices = NULL;
	__pool->uu_slice_c = 0;
	return FFLY_SUCCESS;
}

ff_err_t ffly_pool_de_init(struct ffly_pool *__pool) {
	mem_free(__pool->p);
	mem_free(__pool->uu_slices);
	/*
	ff_err_t err;
	if (_err(err = mem_free(__pool->p))) {
		ffly_fprintf(ffly_err, "pool: failed to free.\n");
		return err;
	}

	if (__pool->uu_slices != NULL) {
		if (_err(err = mem_free(__pool->uu_slices))) {
			ffly_fprintf(ffly_err, "pool: failed to free.");
			return err;
		}
	}
	*/
	return FFLY_SUCCESS;
}

ff_bool_t ffly_pool_full(struct ffly_pool *__pool) {
	return ((__pool->off == __pool->size-1)&&!__pool->uu_slice_c);
}

void* ffly_pool_get(struct ffly_pool *__pool, ff_uint_t __off) {
	return ((ff_u8_t*)__pool->p+(__off*__pool->slice_size));
}

void* ffly_pool_alloc(struct ffly_pool *__pool) {
	if (!__pool->uu_slice_c && __pool->off == __pool->size-1) return NULL;
	void *ret;
	ffly_mutex_lock(&__pool->lock);
	if (__pool->uu_slice_c > 0) {
		ret = *(void**)((ff_u8_t*)__pool->uu_slices+((__pool->uu_slice_c-1)*sizeof(void*)));
		if (!(__pool->uu_slice_c-1)) {
		/*
			if (_err(mem_free(__pool->uu_slices))) {
				ffly_fprintf(ffly_err, "pool: failed to free.\n");
				goto _fatal;
			}
		*/
			mem_free(__pool->uu_slices);
			__pool->uu_slices = NULL;
			__pool->uu_slice_c--;
		} else {
			if ((__pool->uu_slices = (void*)mem_realloc(__pool->uu_slices, ((--__pool->uu_slice_c)*sizeof(void*)))) == NULL) {
				ffly_fprintf(ffly_err, "pool: failed to reallocate memory.\n");
				goto _fatal;
			}
		}
		ffly_mutex_unlock(&__pool->lock);
		return ret;
	}
	ret = (void*)((ff_u8_t*)__pool->p+((__pool->off++)*__pool->slice_size));
	ffly_mutex_unlock(&__pool->lock);
	return ret;
_fatal:
	ffly_mutex_unlock(&__pool->lock);
	return NULL;
}

ff_err_t ffly_pool_free(struct ffly_pool *__pool, void *__p) {
	ff_err_t err = FFLY_SUCCESS;
	ffly_mutex_lock(&__pool->lock);
	if (!__pool->uu_slice_c) {
		if ((__pool->uu_slices = (void*)mem_alloc(sizeof(void*))) == NULL) {
			ffly_fprintf(ffly_err, "pool: failed to allocate memory.\n");
			err = FFLY_FAILURE;
			goto _end;
		}
		__pool->uu_slice_c++;
	} else {
		if ((__pool->uu_slices = (void*)mem_realloc(__pool->uu_slices, (++__pool->uu_slice_c)*sizeof(void*))) == NULL) {
			ffly_fprintf(ffly_err, "pool: failed to reallocate memory.\n");
			err = FFLY_FAILURE;
			goto _end;
		}
	}
	*(void**)((ff_u8_t*)__pool->uu_slices+((__pool->uu_slice_c-1)*sizeof(void*))) = __p;
_end:
	ffly_mutex_unlock(&__pool->lock);
	return err;
}
