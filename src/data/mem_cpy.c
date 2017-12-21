# include "mem_cpy.h"
# include "../system/errno.h"
# ifdef __WITH_TASK_POOL
#	include "../system/task_pool.h"
#	include "../memory/mem_alloc.h"
#	include "../memory/mem_free.h"
#	include "../system/atomic.h"
# endif
void static _ffly_mem_cpy(void const *__dst, void const *__src, mdl_uint_t __bc) {
	mdl_u8_t *src_itr = (mdl_u8_t*)__src;
	while(src_itr != (mdl_u8_t*)__src+__bc) {
		mdl_uint_t off = src_itr-(mdl_u8_t*)__src;
		mdl_uint_t bytes_left = __bc-off;
		if (bytes_left>>3 > 0) {
			*((mdl_u64_t*)((mdl_u8_t*)__dst+off)) = *(mdl_u64_t*)src_itr;
			src_itr+= sizeof(mdl_u64_t);
		} else if (bytes_left>>2 > 0) {
			*((mdl_u32_t*)((mdl_u8_t*)__dst+off)) = *(mdl_u32_t*)src_itr;
			src_itr+= sizeof(mdl_u32_t);
		} else if (bytes_left>>1 > 0) {
			*((mdl_u16_t*)((mdl_u8_t*)__dst+off)) = *(mdl_u16_t*)src_itr;
			src_itr+= sizeof(mdl_u16_t);
		} else
			*((mdl_u8_t*)__dst+off) = *(src_itr++);
	}
}

# ifdef __WITH_TASK_POOL
typedef struct {
	void *dst, *src;
	mdl_uint_t bc;
} mem_cpy_arg_t;

void static* mem_cpy_proxy(void *__arg_p) {
	mem_cpy_arg_t *arg = (mem_cpy_arg_t*)__arg_p;
	_ffly_mem_cpy(arg->dst, arg->src, arg->bc);
}

# define MIN_SLICE_C 2
# define SLICE_SHIFT 20
# endif
ffly_err_t ffly_mem_cpy(void const *__dst, void const *__src, mdl_uint_t __bc) {
# ifdef __WITH_TASK_POOL
	if (__bc>>SLICE_SHIFT >= MIN_SLICE_C && ffly_is_task_pool_inited(&__task_pool__)) {
		mdl_uint_t slice_c = __bc>>SLICE_SHIFT;
		mem_cpy_arg_t *p = __ffly_mem_alloc(slice_c*sizeof(mem_cpy_arg_t));

		mdl_uint_t slice_size = SLICE_SHIFT&0xFF;
		ffly_atomic_uint_t f = 0;
		mdl_u8_t *dst_itr = (mdl_u8_t*)__dst;
		mdl_u8_t *src_itr = (mdl_u8_t*)__src;

		mem_cpy_arg_t *itr = p;
		for (;itr != p+slice_c; itr++) {
			itr->dst = (void*)dst_itr;
			itr->src = (void*)src_itr;
			itr->bc = slice_size;
			ffly_task_pool_add(&__task_pool__, &mem_cpy_proxy, itr, 1, &f);
			dst_itr+= slice_size;
			src_itr+= slice_size;
		}

		mdl_uint_t ebc;
		if ((ebc = (__bc-(slice_c*slice_size))) > 0)
			_ffly_mem_cpy(dst_itr, src_itr, ebc);
		while(f != slice_c);
		__ffly_mem_free(p);
		return;
	}
# endif
	_ffly_mem_cpy(__dst, __src, __bc);
	return FFLY_SUCCESS;
}
