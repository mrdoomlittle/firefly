# include "buff.h"
# include "io.h"
# include "errno.h"
# include "../data/mem_ncpy.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
ffly_err_t ffly_buff_init(struct ffly_buff *__buff, mdl_uint_t __size, mdl_uint_t __blk_size) {
	if ((__buff->p = __ffly_mem_alloc(__size*__blk_size)) == NULL) {
		ffly_printf(stderr, "buff: failed to alloc memory.\n");
		return FFLY_FAILURE;
	}
	__buff->off = 0;
	__buff->size = __buff->rs_size = __size;
	__buff->blk_size = __blk_size;
	return FFLY_SUCCESS;
}

void ffly_buff_put(struct ffly_buff *__buff, void *__p) {
	ffly_mem_ncpy((mdl_u8_t*)__buff->p+(__buff->off*__buff->blk_size), __p, __buff->blk_size);}
void ffly_buff_get(struct ffly_buff *__buff, void *__p) {
	ffly_mem_ncpy(__p, (mdl_u8_t*)__buff->p+(__buff->off*__buff->blk_size), __buff->blk_size);}

ffly_err_t ffly_buff_resize(struct ffly_buff *__buff, mdl_uint_t __nsize) {
	if ((__buff->p = ffly_mem_realloc(__buff->p, __nsize*__buff->blk_size)) == NULL) {
		ffly_printf(stderr, "buff: failed to realloc memory.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_reset(struct ffly_buff *__buff) {
	__buff->off = 0;
	if (ffly_buff_resize(__buff, __buff->rs_size) != FFLY_SUCCESS) {
		ffly_printf(stderr, "buff: failed to resize.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_buff_de_init(struct ffly_buff *__buff) {
	__ffly_mem_free(__buff->p);}
