# include "mem_realloc.h"
# include "mem_free.h"
# include "../system/io.h"
# include "../system/error.h"
# ifdef __ffly_debug
# include "../location.h"
# endif
# ifdef __ffly_mal_track
void* ffly_mem_realloc(void *__p, ff_uint_t __nbc, ff_bool_t __track_bypass) {
# else
void* ffly_mem_realloc(void *__p, ff_uint_t __nbc) {
# endif
# ifdef __ffly_debug
	ff_location_push(_ff_loc_mem_realloc);
# endif
	ff_u8_t *p;
# ifdef __ffly_debug
	p = (ff_u8_t*)__p-sizeof(ff_uint_t);
	ff_uint_t mem_size = *((ff_uint_t*)p);
# ifndef __ffly_use_allocr
	if ((p = (ff_u8_t*)realloc(p, __nbc+sizeof(ff_uint_t))) == NULL) {
		ffly_fprintf(ffly_err, "mem_realloc: failed to reallocate memory.\n");
		goto _fail;
	}
# else
    p = (ff_u8_t*)ffly_brealloc(p, __nbc+sizeof(ff_uint_t));
# endif

	if (__nbc > mem_size)
		ffly_atomic_add(&ffly_mem_alloc_bc, __nbc-mem_size);
	else if (__nbc < mem_size)
		ffly_atomic_sub(&ffly_mem_alloc_bc, mem_size-__nbc);
	*(ff_uint_t*)p = __nbc;
	p+=sizeof(ff_uint_t);
# else
# ifndef __ffly_use_allocr
	p = (ff_u8_t*)realloc(__p, __nbc);
# else
    p = (ff_u8_t*)ffly_brealloc(__p, __nbc);
# endif
# endif

# ifdef __ffly_mal_track
	if (!__track_bypass) {
# ifdef __ffly_debug
		if (
# endif
		ffly_mal_track_update(&__ffly_mal_track__, __p, (void*)p)
# ifdef __ffly_debug
		!= FFLY_SUCCESS) {
			ffly_fprintf(ffly_err, "mem_realloc: mal track failure.\n");
			goto _fail;
		}
# else
;
# endif
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
	return p;
}
