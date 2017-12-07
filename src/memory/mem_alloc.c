# include "mem_alloc.h"
# include "../system/errno.h"
# include "../system/io.h"
# ifdef __DEBUG_ENABLED
ffly_atomic_uint_t ffly_mem_alloc_bc = 0;
ffly_atomic_uint_t ffly_mem_alloc_c = 0;
# endif

# ifdef __MAL_TRACK
void* ffly_mem_alloc(mdl_uint_t __bc, mdl_u8_t __track_bypass) {
# else
void* ffly_mem_alloc(mdl_uint_t __bc) {
# endif /*__MAL_TRACK*/
	mdl_u8_t *p;
# ifdef __DEBUG_ENABLED
	if ((p = (mdl_u8_t*)malloc(__bc+sizeof(mdl_uint_t))) == NULL) {
		ffly_printf(stderr, "mem_alloc: failed to allocate memory.\n");
		return NULL;
	}

	*((mdl_uint_t*)p) = __bc;
	ffly_atomic_add(&ffly_mem_alloc_bc, __bc);
	ffly_atomic_incr(&ffly_mem_alloc_c);
	p += sizeof(mdl_uint_t);
# else
	p = (mdl_u8_t*)malloc(__bc);
# endif
# ifdef __MAL_TRACK
	if (!__track_bypass) {
		if (ffly_mal_track_alloc(&__ffly_mal_track__, (void*)p) != FFLY_SUCCESS) {
# ifdef __DEBUG_ENABLED
			ffly_printf(stderr, "mem_alloc: mal track failure.\n");
# endif
			free((void*)(p+sizeof(mdl_uint_t)));
			return NULL;
		}
	}
# endif
	return (void*)p;
}
