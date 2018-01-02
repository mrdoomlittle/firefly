# include "mem_cpy.h"
# include "../system/errno.h"
# include "../types/off_t.h"
# ifdef __ffly_use_task_pool
#	include "../system/task_pool.h"
#	include "../memory/mem_alloc.h"
#	include "../memory/mem_free.h"
#	include "../system/atomic.h"
# endif
void static _ffly_mem_cpy(void *__dst, void *__src, mdl_uint_t __bc) {
	if (__bc == sizeof(mdl_u8_t)) {
		*(mdl_u8_t*)__dst = *(mdl_u8_t*)__src;
		return;
	} else if (__bc == sizeof(mdl_u64_t)) {
		*(mdl_u64_t*)__dst = *(mdl_u64_t*)__src;
		return;
	} else if (__bc == sizeof(mdl_u32_t)) {
		*(mdl_u32_t*)__dst = *(mdl_u32_t*)__src;
		return;
	} else if (__bc == sizeof(mdl_u16_t)) {
		*(mdl_u16_t*)__dst = *(mdl_u16_t*)__src;
		return;
	}

	mdl_u8_t *itr = (mdl_u8_t*)__src;
	while(itr != (mdl_u8_t*)__src+__bc) {
		ffly_off_t off = itr-(mdl_u8_t*)__src;
		mdl_uint_t left = __bc-off;
		if (left>>3 > 0) {
			*((mdl_u64_t*)((mdl_u8_t*)__dst+off)) = *(mdl_u64_t*)itr;
			itr+= sizeof(mdl_u64_t);
		} else if (left>>2 > 0) {
			*((mdl_u32_t*)((mdl_u8_t*)__dst+off)) = *(mdl_u32_t*)itr;
			itr+= sizeof(mdl_u32_t);
		} else if (left>>1 > 0) {
			*((mdl_u16_t*)((mdl_u8_t*)__dst+off)) = *(mdl_u16_t*)itr;
			itr+= sizeof(mdl_u16_t);
		} else
			*((mdl_u8_t*)__dst+off) = *(itr++);
	}
}

# ifdef __ffly_use_task_pool
typedef struct {
	void *dst, *src;
	mdl_uint_t bc;
    ffly_atomic_uint_t *done;
} mem_cpy_arg_t;

mdl_i8_t static proxy(void *__arg_p) {
	mem_cpy_arg_t *arg = (mem_cpy_arg_t*)__arg_p;
	_ffly_mem_cpy(arg->dst, arg->src, arg->bc);
    ffly_atomic_incr(arg->done);
    return 0; // exit
}

# define MIN_SLICE_C 2
# define SLICE_SHIFT 20
# endif
ffly_err_t ffly_mem_cpy(void *__dst, void *__src, mdl_uint_t __bc) {
# ifdef __ffly_use_task_pool
	if (__bc>>SLICE_SHIFT >= MIN_SLICE_C) {
		mdl_uint_t slice_c = __bc>>SLICE_SHIFT;
		mem_cpy_arg_t *p = __ffly_mem_alloc(slice_c*sizeof(mem_cpy_arg_t));

		mdl_uint_t slice_size = SLICE_SHIFT&0xFF;
		ffly_atomic_uint_t done = 0;
		mdl_u8_t *dst_itr = (mdl_u8_t*)__dst;
		mdl_u8_t *src_itr = (mdl_u8_t*)__src;

		mem_cpy_arg_t *itr = p;
		for (;itr != p+slice_c; itr++) {
			itr->dst = (void*)dst_itr;
			itr->src = (void*)src_itr;
			itr->bc = slice_size;
            itr->done = &done;
			ffly_task_pool_add(&__ffly_task_pool__, &proxy, itr);
			dst_itr+= slice_size;
			src_itr+= slice_size;
		}

		mdl_uint_t extra;
		if ((extra = (__bc-(slice_c*slice_size))) > 0)
			_ffly_mem_cpy(dst_itr, src_itr, extra);
		while(done != slice_c);
		__ffly_mem_free(p);
		return FFLY_SUCCESS;
	}
# endif
	_ffly_mem_cpy(__dst, __src, __bc);
	return FFLY_SUCCESS;
}
