# include "mem_alloc.h"
# include "mem_free.h"
# include "../system/errno.h"
# include "../system/io.h"
# ifdef __ffly_debug
# include "../location.h"
ff_u64_t ffly_mem_alloc_bc = 0;
ff_u64_t ffly_mem_alloc_c = 0;
# endif

void static* _memalloc(ff_uint_t __n) {
#ifndef __ffly_use_allocr
	return malloc(__n);
#else
	return ffly_balloc(__n);
#endif
}

void*(*__memalloc)(ff_uint_t) = _memalloc;

# ifdef __ffly_mal_track
void* ffly_mem_alloc(ff_uint_t __bc, ff_u8_t __track_bypass) {
# else
void* ffly_mem_alloc(ff_uint_t __bc) {
# ifdef __ffly_debug
	ff_location_push(_ff_loc_mem_alloc);
# endif
# endif /*__ffly_mal_track*/
	ff_u8_t *p;
# ifdef __ffly_debug
	if ((p = (ff_u8_t*)__memalloc(__bc+sizeof(ff_uint_t))) == NULL) {
		ffly_fprintf(ffly_err, "mem_alloc: failed to allocate memory.\n");
		goto _fail;
	}

	*((ff_uint_t*)p) = __bc;
	__asm__(
#if (!defined(__ff32) && !defined(__ff64)) || defined(__ff32)
			"xorq %%rax, %%rax\n\t"
			"movl %2, %%eax\n\t"
#elif defined(__ff64)
			"movq %2, %%rax\n\t"
#else
#error "error"
#endif
			"lock addq %%rax, %0\n\t"
			"lock incq %1" :
				"=m"(ffly_mem_alloc_bc),
					"=m"(ffly_mem_alloc_c) :
						"r"(__bc) : "rax");
	p+=sizeof(ff_uint_t);
# else
	p = (ff_u8_t*)__memalloc(__bc);
# endif
# ifdef __ffly_mal_track
	if (!__track_bypass) {
		if (ffly_mal_track_alloc(&__ffly_mal_track__, (void*)p) != FFLY_SUCCESS) {
# ifdef __ffly_debug
			ffly_fprintf(ffly_err, "mem_alloc: mal track failure.\n");
# endif
			__memfree((void*)(p-sizeof(ff_uint_t)));
			goto _fail;
		}
	}
# endif
	goto _succ;
_fail:
	if (p != NULL)
		__ffly_mem_free(p);
	p = NULL;
_succ:
# ifdef __ffly_debug
	ff_location_pop();
# endif
	return (void*)p;
}

void* ffly_mal() {
# ifdef __ffly_mal_track
	register ff_u8_t bypass __asm__("r8");
# endif
	register ff_uint_t bc __asm__("r9") = 0;
	__asm__(
#if (!defined(__ff32) && !defined(__ff64)) || defined(__ff32)
		"movl 16(%%rbp), %%eax\n\t"
		"movl %%eax, %0\n\t"
# ifdef __ffly_mal_track
		"movb 20(%%rbp), %%bl\n\t"
# endif
#elif defined(__ff64)
		"movq 16(%%rbp), %%rax\n\t"
		"movq %%rax, %0\n\t"
# ifdef __ffly_mal_track
		"movb 24(%%rbp), %%bl\n\t"
# endif
#else
#error "error"
#endif
# ifdef __ffly_mal_track
		"movb %%bl, %1\n\t"
# endif
	: "=r"(bc)
# ifdef __ffly_mal_track
	, "=r"(bypass)
# endif
	: : "rax", "bl"
);
	register void *ret __asm__("rax");
	ret = ffly_mem_alloc(bc
# ifdef __ffly_mal_track
	, bypass
# endif
);
	return ret;
}
