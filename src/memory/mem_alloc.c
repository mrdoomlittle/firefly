# include "mem_alloc.h"
# include "../system/errno.h"
# include "../system/io.h"
# ifdef __ffly_debug_enabled
ffly_atomic_uint_t ffly_mem_alloc_bc = 0;
ffly_atomic_uint_t ffly_mem_alloc_c = 0;
# endif

# ifdef __ffly_mal_track
void* ffly_mem_alloc(mdl_uint_t __bc, mdl_u8_t __track_bypass) {
# else
void* ffly_mem_alloc(mdl_uint_t __bc) {
# endif /*__ffly_mal_track*/
	mdl_u8_t *p;
# ifdef __ffly_debug_enabled
# ifndef __ffly_use_allocr
	if ((p = (mdl_u8_t*)malloc(__bc+sizeof(mdl_uint_t))) == NULL) {
		ffly_printf(stderr, "mem_alloc: failed to allocate memory.\n");
		return NULL;
	}
# else
    p = ffly_alloc(__bc+sizeof(mdl_uint_t));
# endif

	*((mdl_uint_t*)p) = __bc;
	ffly_atomic_add(&ffly_mem_alloc_bc, __bc);
	ffly_atomic_incr(&ffly_mem_alloc_c);
	p += sizeof(mdl_uint_t);
# else
# ifndef __ffly_use_allocr
	p = (mdl_u8_t*)malloc(__bc);
# else
    p = ffly_alloc(__bc);
# endif
# endif
# ifdef __ffly_mal_track
	if (!__track_bypass) {
		if (ffly_mal_track_alloc(&__ffly_mal_track__, (void*)p) != FFLY_SUCCESS) {
# ifdef __ffly_debug_enabled
			ffly_printf(stderr, "mem_alloc: mal track failure.\n");
# endif
# ifndef __ffly_use_allocr
			free((void*)(p+sizeof(mdl_uint_t)));
# else
            ffly_free((void*)(p+sizeof(mdl_uint_t)));
# endif
			return NULL;
		}
	}
# endif
	return (void*)p;
}
