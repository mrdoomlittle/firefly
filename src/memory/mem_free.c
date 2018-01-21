# include "mem_free.h"
# include "../system/io.h"
# include "../system/errno.h"
# ifdef __ffly_debug_enabled
ffly_atomic_uint_t ffly_mem_free_bc = 0;
ffly_atomic_uint_t ffly_mem_free_c = 0;
# endif
# ifdef __ffly_mal_track
ffly_err_t ffly_mem_free(void *__p, ffly_bool_t __track_bypass) {
	ffly_err_t any_err;
	if (!__track_bypass) {
# ifdef __ffly_debug_enabled
		if ((
# endif
		any_err = ffly_mal_track_free(&__ffly_mal_track__, __p, 0)
# ifdef __ffly_debug_enabled
		) != FFLY_SUCCESS) {
			ffly_fprintf(ffly_err, "mem_free: mal track failure.\n");
		}
# else
;
# endif
	}
# else
ffly_err_t ffly_mem_free(void *__p) {
# endif
# ifdef __ffly_debug_enabled
	mdl_u8_t *p = (mdl_u8_t*)__p-sizeof(mdl_uint_t);
	ffly_atomic_add(&ffly_mem_free_bc, *(mdl_uint_t*)p);
	ffly_atomic_incr(&ffly_mem_free_c);
# ifndef __ffly_use_allocr
	free((void*)p);
# else
    ffly_free((void*)p);
# endif
# else
# ifndef __ffly_use_allocr
	free(__p);
# else
    ffly_free(__p);
# endif
# endif
# ifdef __ffly_mal_track
	return any_err;
# else
	return FFLY_SUCCESS;
# endif
}
