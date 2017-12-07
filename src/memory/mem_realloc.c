# include "mem_realloc.h"
# include "../system/io.h"
# ifdef __MAL_TRACK
void* ffly_mem_realloc(void *__p, mdl_uint_t __nbc, ffly_bool_t __track_bypass) {
# else
void* ffly_mem_realloc(void *__p, mdl_uint_t __nbc) {
# endif
	mdl_u8_t *p;
# ifdef __DEBUG_ENABLED
	p = (mdl_u8_t*)__p-sizeof(mdl_uint_t);
	mdl_uint_t mem_size = *((mdl_uint_t*)p);

	if ((p = (mdl_u8_t*)realloc(p, __nbc+sizeof(mdl_uint_t))) == NULL) {
		ffly_printf(stderr, "mem_realloc: failed to reallocate memory.\n");
		return NULL;
	}

	if (__nbc > mem_size)
		ffly_atomic_add(&ffly_mem_alloc_bc, __nbc-mem_size);
	else if (__nbc < mem_size)
		ffly_atomic_sub(&ffly_mem_alloc_bc, mem_size-__nbc);
	*((mdl_uint_t*)p) = __nbc;
	p += sizeof(mdl_uint_t);
# else
	p = (mdl_u8_t*)realloc(__p, __nbc);
# endif

# ifdef __MAL_TRACK
	if (!__track_bypass) {
# ifdef __DEBUG_ENABLED
		if (
# endif
		ffly_mal_track_update(&__ffly_mal_track__, __p, (void*)p)
# ifdef __DEBUG_ENABLED
		!= FFLY_SUCCESS) {
			ffly_printf(stderr, "mem_realloc: mal track failure.\n");
		}
# else
;
# endif

	}
# endif
	return p;
}
