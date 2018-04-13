# include "mem_realloc.h"
# include "../system/io.h"
# ifdef __ffly_mal_track
void* ffly_mem_realloc(void *__p, ff_uint_t __nbc, ff_bool_t __track_bypass) {
# else
void* ffly_mem_realloc(void *__p, ff_uint_t __nbc) {
# endif
	ff_u8_t *p;
# ifdef __ffly_debug_enabled
	p = (ff_u8_t*)__p-sizeof(ff_uint_t);
	ff_uint_t mem_size = *((ff_uint_t*)p);
# ifndef __ffly_use_allocr
	if ((p = (ff_u8_t*)realloc(p, __nbc+sizeof(ff_uint_t))) == NULL) {
		ffly_fprintf(ffly_err, "mem_realloc: failed to reallocate memory.\n");
		return NULL;
	}
# else
    p = (ff_u8_t*)ffly_realloc(p, __nbc+sizeof(ff_uint_t));
# endif

	if (__nbc > mem_size)
		ffly_atomic_add(&ffly_mem_alloc_bc, __nbc-mem_size);
	else if (__nbc < mem_size)
		ffly_atomic_sub(&ffly_mem_alloc_bc, mem_size-__nbc);
	*(ff_uint_t*)p = __nbc;
	p += sizeof(ff_uint_t);
# else
# ifndef __ffly_use_allocr
	p = (ff_u8_t*)realloc(__p, __nbc);
# else
    p = (ff_u8_t*)ffly_realloc(__p, __nbc);
# endif
# endif

# ifdef __ffly_mal_track
	if (!__track_bypass) {
# ifdef __ffly_debug_enabled
		if (
# endif
		ffly_mal_track_update(&__ffly_mal_track__, __p, (void*)p)
# ifdef __ffly_debug_enabled
		!= FFLY_SUCCESS) {
			ffly_fprintf(ffly_err, "mem_realloc: mal track failure.\n");
		}
# else
;
# endif

	}
# endif
	return p;
}
