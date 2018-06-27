# include "mem_free.h"
# include "../system/io.h"
# include "../system/errno.h"
# ifdef __ffly_debug
# include "../location.h"
ff_atomic_uint_t ffly_mem_free_bc = 0;
ff_atomic_uint_t ffly_mem_free_c = 0;
# endif
# ifdef __ffly_mal_track
ff_err_t ffly_mem_free(void *__p, ff_bool_t __track_bypass) {
	ff_err_t err;
	ff_u8_t *p;
	if (!__track_bypass) {
# ifdef __ffly_debug
		if ((
# endif
		err = ffly_mal_track_free(&__ffly_mal_track__, __p, 0)
# ifdef __ffly_debug
		) != FFLY_SUCCESS) {
			ffly_fprintf(ffly_err, "mem_free: mal track failure.\n");
			goto _fail;
		}
# else
;
# endif
	}
# else
ff_err_t ffly_mem_free(void *__p) {
	ff_err_t err;
# endif
# ifdef __ffly_debug
	ff_location_push(_ff_loc_mem_free);
# ifndef __ffly_mal_track
	ff_u8_t *
# endif
	p = (ff_u8_t*)__p-sizeof(ff_uint_t);
	ffly_atomic_add(&ffly_mem_free_bc, *(ff_uint_t*)p);
	ffly_atomic_incr(&ffly_mem_free_c);
# ifndef __ffly_use_allocr
	free((void*)p);
# else
    ffly_bfree((void*)p);
# endif
# else
# ifndef __ffly_use_allocr
	free(__p);
# else
    ffly_bfree(__p);
# endif
# endif
	err = FFLY_SUCCESS;
	goto _succ;
_fail:
	err = FFLY_FAILURE;
_succ:
# ifdef __ffly_debug
	ff_location_pop();
# endif
	return err;
}
