# include "mem_free.h"
# include "../system/io.h"
# include "../system/errno.h"
# ifdef __DEBUG_ENABLED
ffly_atomic_uint_t ffly_mem_free_bc;
ffly_atomic_uint_t ffly_mem_free_c;
# endif
# ifdef __MAL_TRACK
ffly_err_t ffly_mem_free(void *__p, ffly_bool_t __track_bypass) {
	ffly_err_t any_err;
	if (!__track_bypass) {
# ifdef __DEBUG_ENABLED
		if ((
# endif
		any_err = ffly_mal_track_free(&__ffly_mal_track__, __p, 0)
# ifdef __DEBUG_ENABLED
		) != FFLY_SUCCESS) {
			ffly_printf(stderr, "mem_free: mal track failure.\n");
		}
# else
;
# endif
	}
# else
ffly_err_t ffly_mem_free(void *__p) {
# endif
# ifdef __DEBUG_ENABLED
	mdl_u8_t *p = ((mdl_u8_t*)__p)-sizeof(mdl_uint_t);
	ffly_atomic_add(&ffly_mem_free_bc, *(mdl_uint_t*)p);
	ffly_atomic_incr(&ffly_mem_free_c);
	free((void*)p);
# else
	free(__p);
# endif
# ifdef __MAL_TRACK
	return any_err;
# else
	return FFLY_SUCCESS;
# endif
}
