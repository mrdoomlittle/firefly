# include "mem_free.h"
# include "../system/io.h"
# include "../system/errno.h"
# ifdef __DEBUG_ENABLED
ffly_atomic_uint_t ffly_mem_free_bc;
ffly_atomic_uint_t ffly_mem_free_c;
# endif
# ifdef __WITH_MEM_TRACKER
ffly_err_t ffly_mem_free(void *__p, ffly_bool_t __track_bypass) {
	ffly_err_t any_err;
	if (!__track_bypass) {
# ifdef __DEBUG_ENABLED
		if ((
# endif
		any_err = ffly_mem_track_free(&__ffly_mem_track__, __p, 0)
# ifdef __DEBUG_ENABLED
		) != FFLY_SUCCESS) {
			ffly_printf(stderr, "mem_free: mem tracker failure.\n");
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
# ifdef __WITH_MEM_TRACKER
	return any_err;
# else
	return FFLY_SUCCESS;
# endif
}
