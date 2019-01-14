# include "buff.h"
# include "io.h"
# include "err.h"
# include "errno.h"
# include "../dep/mem_cpy.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "mutex.h"
#define __FSPS __buff
#ifndef FF_BUFF_SA
#define mem_alloc(__n)\
    __ffly_mem_alloc(__n)   
#define mem_free(__p)\
    __ffly_mem_free(__p)
#define mem_realloc(__p, __n)\
    __ffly_mem_realloc(__p, __n)
#else
#define mem_alloc(__n)\
    __FSPS->alloc(__FSPS->ma_arg, __n)
#define mem_free(__p)\
    __FSPS->free(__FSPS->ma_arg, __p)
#define mem_realloc(__p, __n)\
    __FSPS->realloc(__FSPS->ma_arg, __p, __n)
#endif

void static* dummy_alloc(long long __arg, ff_uint_t __n) {
	return __ffly_mem_alloc(__n);
}
void static dummy_free(long long __arg, void *__p) {
	__ffly_mem_free(__p);
}
void static* dummy_realloc(long long __arg, void *__p, ff_uint_t __n) {
	return __ffly_mem_realloc(__p, __n);
}

ff_err_t _ffly_buff_init(ffly_buffp __FSPS, ff_uint_t __blk_c, ff_size_t __blk_size, void(*__grab)(long long, ff_u8_t)) {
#ifdef FF_BUFF_SA
	if (__grab != NULL) {
		__grab(&__FSPS->alloc, FF_BGF_MA);
		__grab(&__FSPS->free, FF_BGF_MF);
		__grab(&__FSPS->realloc, FF_BGF_MR);
		goto _sk:
	}
	__FSPS->alloc = dummy_alloc;
	__FSPS->free = dummy_free;
	__FSPS->realloc = dummy_realloc;
_sk:
#endif

	if ((__FSPS->p = mem_alloc(__blk_c*__blk_size)) == NULL) {
		ffly_fprintf(ffly_err, "buff: failed to allocate memory.\n");
		return FFLY_FAILURE;
	}
	__FSPS->lock = FFLY_MUTEX_INIT;
	__FSPS->off = 0;
	__FSPS->blk_c = __blk_c;
	__FSPS->rs_blk_c = __blk_c;
	__FSPS->blk_size = __blk_size;
	return FFLY_SUCCESS;
}

void* ffly_buff_getp(ffly_buffp __FSPS) {
	return (ff_u8_t*)__FSPS->p+(__FSPS->off*__FSPS->blk_size);
}

void* ffly_buff_at(ffly_buffp __FSPS, ff_uint_t __off) {
	return (ff_u8_t*)__FSPS->p+(__off*__FSPS->blk_size);
}

ff_err_t ffly_buff_put(ffly_buffp __FSPS, void *__p) {
	ff_err_t err;
	ffly_mutex_lock(&__FSPS->lock);
	if (_err(err = ffly_mem_cpy((ff_u8_t*)__FSPS->p+(__FSPS->off*__FSPS->blk_size), __p, __FSPS->blk_size)))
		ffly_fprintf(ffly_err, "buff: failed to copy blk.\n");
	ffly_mutex_unlock(&__FSPS->lock);
	return FFLY_SUCCESS;
}

ff_err_t ffly_buff_get(ffly_buffp __FSPS, void *__p) {
	ff_err_t err;
	ffly_mutex_lock(&__FSPS->lock);
	if (_err(err = ffly_mem_cpy(__p, (ff_u8_t*)__FSPS->p+(__FSPS->off*__FSPS->blk_size), __FSPS->blk_size)))
		ffly_fprintf(ffly_err, "buff: failed to copy blk.\n");
	ffly_mutex_unlock(&__FSPS->lock);
	return FFLY_SUCCESS;
}

ff_err_t ffly_buff_incr(ffly_buffp __FSPS) {
	ff_err_t err = FFLY_SUCCESS;
	ffly_mutex_lock(&__FSPS->lock);
	if (__FSPS->off == __FSPS->blk_c-1) {
		ffly_fprintf(ffly_err, "buff: can't incrment any further.\n");
		err = FFLY_FAILURE;
	} else __FSPS->off++;
	ffly_mutex_unlock(&__FSPS->lock);
	return err;
}

ff_err_t ffly_buff_decr(ffly_buffp __FSPS) {
	ff_err_t err = FFLY_SUCCESS;
	ffly_mutex_lock(&__FSPS->lock);
	if (!__FSPS->off) {
		ffly_fprintf(ffly_err, "buff: can't de-incrment any further.\n");
		err =  FFLY_FAILURE;
	} else __FSPS->off--;
	ffly_mutex_unlock(&__FSPS->lock);
	return err;
}

ff_err_t ffly_buff_resize(ffly_buffp __FSPS, ff_uint_t __blk_c) {
	if ((__FSPS->p = __ffly_mem_realloc(__FSPS->p, (__FSPS->blk_c = __blk_c)*__FSPS->blk_size)) == NULL) {
		ffly_fprintf(ffly_err, "buff: failed to realloc memory.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ff_err_t ffly_buff_reset(ffly_buffp __FSPS) {
	ff_err_t err;
	__FSPS->off = 0;
	if (_err(err = ffly_buff_resize(__FSPS, __FSPS->rs_blk_c))) {
		ffly_fprintf(ffly_err, "buff: failed to resize.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ff_err_t ffly_buff_de_init(ffly_buffp __FSPS) {
	ff_err_t err;
	if (_err(err = __ffly_mem_free(__FSPS->p))) {
		ffly_fprintf(ffly_err, "buff: failed to free.\n");
		return err;
	}
	return FFLY_SUCCESS;
}
