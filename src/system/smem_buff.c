# include "smem_buff.h"
struct ffly_smem_buff_t* ffly_smem_buff_ninst(mdl_uint_t __buff_size, mdl_uint_t __blk_len) {
	struct ffly_smem_buff_t* tr = (struct ffly_smem_buff_t*)__ffly_mem_alloc(sizeof(struct ffly_smem_buff_t));
	ffly_smem_buff_init(tr, __buff_size, __blk_len);
	return tr;
}

ffly_err_t ffly_smem_buff_init(struct ffly_smem_buff_t *__smem_buff, mdl_uint_t __buff_size, mdl_uint_t __blk_len) {
	__smem_buff-> blk_len = __blk_len;
	__smem_buff-> buff_size = __buff_size;
	__smem_buff-> nxt_point = 0;
	if ((__smem_buff-> dbuff = (mdl_u8_t*)__ffly_mem_alloc(__buff_size*__blk_len)) == NULL) {
		fprintf(stderr, "smem_buff: failed to alloc memory for buff, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}

ffly_err_t ffly_smem_buff_push(struct ffly_smem_buff_t *__smem_buff, void *__dptr) {
	if (!__smem_buff-> dbuff) return FFLY_FAILURE;
	if (__smem_buff-> nxt_point == __smem_buff-> buff_size - 1) {
		ffly_errno = FF_ERR_SMBF;
		return FFLY_FAILURE;
	}

	mdl_uint_t buff_fs = __smem_buff-> nxt_point*__smem_buff-> blk_len;
	mdl_u8_t *d = __smem_buff-> dbuff + buff_fs;
	for (mdl_u8_t* itor = d; itor != d+__smem_buff-> blk_len; itor++, __dptr++) *itor = *(mdl_u8_t*)__dptr;
	__smem_buff-> nxt_point++;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_smem_buff_ptr(struct ffly_smem_buff_t *__smem_buff, void **__ptr) {
	if (!__smem_buff-> dbuff) return FFLY_FAILURE;
	if (__smem_buff-> nxt_point - __smem_buff-> blk_len < 0) return FFLY_FAILURE;
	if (__smem_buff-> nxt_point + __smem_buff-> blk_len > __smem_buff-> buff_size) return FFLY_FAILURE;
	*__ptr = __smem_buff-> dbuff + ((__smem_buff-> nxt_point - 1)*__smem_buff-> blk_len);
}

ffly_err_t ffly_smem_buff_pop(struct ffly_smem_buff_t *__smem_buff, void *__dptr) {
	if (!__smem_buff-> dbuff) return FFLY_FAILURE;
	if (!__smem_buff-> nxt_point) {
		ffly_errno = FF_ERR_SMBE;
		return FFLY_FAILURE;
	}

	if (__dptr != NULL) {
		mdl_uint_t buff_fs = (__smem_buff-> nxt_point-1)*__smem_buff-> blk_len;
		mdl_u8_t *d = __smem_buff-> dbuff + buff_fs;
		for (mdl_u8_t* itor = d; itor != d+__smem_buff-> blk_len; itor++, __dptr++) *(mdl_u8_t*)__dptr = *itor;
	}
	__smem_buff-> nxt_point--;
	return FFLY_SUCCESS;
}
