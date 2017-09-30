# include "buff.h"
# include "io.h"
# include "errno.h"
# include "../data/mem_cpy.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "mutex.h"
ffly_err_t ffly_buff_init(struct ffly_buff *__buff, mdl_uint_t __blk_c, mdl_uint_t __blk_size) {
	if ((__buff->p = __ffly_mem_alloc(__blk_c*__blk_size)) == NULL) {
		ffly_printf(stderr, "buff: failed to alloc memory.\n");
		return FFLY_FAILURE;
	}
	__buff->m = FFLY_MUTEX_INIT;
	__buff->off = 0;
	__buff->blk_c = __buff->rs_blk_c = __blk_c;
	__buff->blk_size = __blk_size;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_put(struct ffly_buff *__buff, void *__p) {
	ffly_mutex_lock(&__buff->m);
	ffly_mem_cpy((mdl_u8_t*)__buff->p+(__buff->off*__buff->blk_size), __p, __buff->blk_size);
	ffly_mutex_unlock(&__buff->m);
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_get(struct ffly_buff *__buff, void *__p) {
	ffly_mem_cpy(__p, (mdl_u8_t*)__buff->p+(__buff->off*__buff->blk_size), __buff->blk_size);
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_incr(struct ffly_buff *__buff) {
	ffly_mutex_lock(&__buff->m);
	if (__buff->off+1 > __buff->blk_c) return FFLY_FAILURE;
	__buff->off++;
	ffly_mutex_unlock(&__buff->m);
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_decr(struct ffly_buff *__buff) {
	ffly_mutex_lock(&__buff->m);
	if (!__buff->off) return FFLY_FAILURE;
	__buff->off--;
	ffly_mutex_unlock(&__buff->m);
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_resize(struct ffly_buff *__buff, mdl_uint_t __nsize) {
	if ((__buff->p = __ffly_mem_realloc(__buff->p, __nsize*__buff->blk_size)) == NULL) {
		ffly_printf(stderr, "buff: failed to realloc memory.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_reset(struct ffly_buff *__buff) {
	__buff->off = 0;
	if (ffly_buff_resize(__buff, __buff->rs_blk_c) != FFLY_SUCCESS) {
		ffly_printf(stderr, "buff: failed to resize.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_de_init(struct ffly_buff *__buff) {
	__ffly_mem_free(__buff->p);}
