# include "buff.h"
# include "io.h"
# include "err.h"
# include "errno.h"
# include "../dep/mem_cpy.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "mutex.h"
ffly_err_t ffly_buff_init(ffly_buffp __buff, mdl_uint_t __blk_c, ffly_size_t __blk_size) {
	if ((__buff->p = __ffly_mem_alloc(__blk_c*__blk_size)) == NULL) {
		ffly_fprintf(ffly_err, "buff: failed to allocate memory.\n");
		return FFLY_FAILURE;
	}
	__buff->m = FFLY_MUTEX_INIT;
	__buff->off = 0;
	__buff->blk_c = __blk_c;
	__buff->rs_blk_c = __blk_c;
	__buff->blk_size = __blk_size;
	return FFLY_SUCCESS;
}

void* ffly_buff_getp(ffly_buffp __buff) {
	return (mdl_u8_t*)__buff->p+(__buff->off*__buff->blk_size);
}

void* ffly_buff_at(ffly_buffp __buff, mdl_uint_t __off) {
	return (mdl_u8_t*)__buff->p+(__off*__buff->blk_size);
}

ffly_err_t ffly_buff_put(ffly_buffp __buff, void *__p) {
	ffly_err_t err;
	ffly_mutex_lock(&__buff->m);
	if (_err(err = ffly_mem_cpy((mdl_u8_t*)__buff->p+(__buff->off*__buff->blk_size), __p, __buff->blk_size)))
		ffly_fprintf(ffly_err, "buff: failed to copy blk.\n");
	ffly_mutex_unlock(&__buff->m);
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_get(ffly_buffp __buff, void *__p) {
	ffly_err_t err;
	ffly_mutex_lock(&__buff->m);
	if (_err(err = ffly_mem_cpy(__p, (mdl_u8_t*)__buff->p+(__buff->off*__buff->blk_size), __buff->blk_size)))
		ffly_fprintf(ffly_err, "buff: failed to copy blk.\n");
	ffly_mutex_unlock(&__buff->m);
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_incr(ffly_buffp __buff) {
	ffly_err_t err = FFLY_SUCCESS;
	ffly_mutex_lock(&__buff->m);
	if (__buff->off == __buff->blk_c-1) {
		ffly_fprintf(ffly_err, "buff: can't incrment any further.\n");
		err = FFLY_FAILURE;
	} else __buff->off++;
	ffly_mutex_unlock(&__buff->m);
	return err;
}

ffly_err_t ffly_buff_decr(ffly_buffp __buff) {
	ffly_err_t err = FFLY_SUCCESS;
	ffly_mutex_lock(&__buff->m);
	if (!__buff->off) {
		ffly_fprintf(ffly_err, "buff: can't de-incrment any further.\n");
		err =  FFLY_FAILURE;
	} else __buff->off--;
	ffly_mutex_unlock(&__buff->m);
	return err;
}

ffly_err_t ffly_buff_resize(ffly_buffp __buff, mdl_uint_t __blk_c) {
	if ((__buff->p = __ffly_mem_realloc(__buff->p, (__buff->blk_c = __blk_c)*__buff->blk_size)) == NULL) {
		ffly_fprintf(ffly_err, "buff: failed to realloc memory.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_reset(ffly_buffp __buff) {
	ffly_err_t err;
	__buff->off = 0;
	if (_err(err = ffly_buff_resize(__buff, __buff->rs_blk_c))) {
		ffly_fprintf(ffly_err, "buff: failed to resize.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_de_init(ffly_buffp __buff) {
	ffly_err_t err;
	if (_err(err = __ffly_mem_free(__buff->p))) {
		ffly_fprintf(ffly_err, "buff: failed to free.\n");
		return err;
	}
	return FFLY_SUCCESS;
}
