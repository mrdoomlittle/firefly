# include "mem_set.h"
# include "../system/err.h"
# include "../system/io.h"
# ifndef __ffly_no_task_pool
#   include "../types/off_t.h"
#   include "../system/task_pool.h"
#   include "../memory/mem_alloc.h"
#   include "../memory/mem_free.h"
#   include "../system/atomic.h"
#   include "../system/nanosleep.h"
# endif
void static * _ffly_mem_set(void *__dst, mdl_u8_t __val, mdl_uint_t __bc) {
    mdl_u8_t _8l = __val;
    mdl_u16_t _16l = _8l|_8l<<8;
    mdl_u32_t _32l = _16l|((mdl_u32_t)_16l)<<16;
    mdl_u64_t _64l = _32l|((mdl_u64_t)_32l)<<32;
    if (__bc == sizeof(mdl_u8_t)) {
        *(mdl_u8_t*)__dst = _8l;
        goto _end;
    } else if (__bc == sizeof(mdl_u64_t)) {
        *(mdl_u64_t*)__dst = _64l;
        goto _end;
    } else if (__bc == sizeof(mdl_u32_t)) {
        *(mdl_u32_t*)__dst = _32l;
        goto _end;
    } else if (__bc == sizeof(mdl_u16_t)) {
        *(mdl_u16_t*)__dst = _16l;
        goto _end;
    }

	mdl_u8_t *p = (mdl_u8_t*)__dst;
    mdl_u8_t *end = p+__bc;
	while(p != end) {
        mdl_uint_t left = __bc-(p-(mdl_u8_t*)__dst);
        if (left>>3 > 0) {
            *(mdl_u64_t*)p = _64l;
            p+= sizeof(mdl_u64_t);
        } else if (left>>2 > 0) {
            *(mdl_u32_t*)p = _32l;
            p+= sizeof(mdl_u32_t);
        } else if (left>>1 > 0) {
            *(mdl_u16_t*)p = _16l;
            p+= sizeof(mdl_u16_t);
        } else
		    *(p++) = __val;
    }
    _end:
	return __dst;
}

# ifndef __ffly_no_task_pool
typedef struct {
    void *dst;
    mdl_u8_t val;
    mdl_uint_t bc;
    ffly_atomic_uint_t *done;
} arg;


mdl_i8_t proxy(void *__arg_p) {
    arg *p = (arg*)__arg_p;
    _ffly_mem_set(p->dst, p->val, p->bc);
    ffly_atomic_incr(p->done);
    return 0;
}

# define MIN_SLICE_C 2
# define SLICE_SHIFT 13
# endif
void *ffly_mem_set(void *__dst, mdl_u8_t __val, mdl_uint_t __bc) {
# ifndef __ffly_no_task_pool
    if ((__bc>>SLICE_SHIFT) >= MIN_SLICE_C) {
        mdl_uint_t slice_c = __bc>>SLICE_SHIFT;
        arg *p = __ffly_mem_alloc(slice_c*sizeof(arg));

        mdl_uint_t slice_size = 1<<SLICE_SHIFT;
        ffly_atomic_uint_t done = 0;
        mdl_u8_t *dst = (mdl_u8_t*)__dst;
        arg *itr = p;
        for (;itr != p+slice_c;itr++) {
            itr->dst = dst;
            itr->bc = slice_size;
            itr->val = __val;
            itr->done = &done;
            if (_err(ffly_task_pool_add(&__ffly_task_pool__, &proxy, itr))) {
                ffly_fprintf(ffly_err, "failed to add task to pool.\n");
            }
            dst+= slice_size; 
        }

        mdl_uint_t extra;
        if ((extra = (__bc-(slice_c*slice_size))) > 0)
            _ffly_mem_set(dst, __val, extra);
        while(done != slice_c);
        ffly_mem_free(p);
        return __dst;
    } else
# endif
        return _ffly_mem_set(__dst, __val, __bc);
}
