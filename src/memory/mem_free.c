# include "mem_free.h"
# ifdef __DEBUG_ENABLED
mdl_uint_t ffly_mem_free_bc;
mdl_uint_t ffly_mem_free_c;
# endif

# ifdef __WITH_MEM_TRACKER
void ffly_mem_free(void *__mem_ptr, ffly_bool_t __track_bypass) {
	if (!__track_bypass)
		ffly_mem_track_free(&__ffly_mem_track__, __mem_ptr, 0);
# else
void ffly_mem_free(void *__mem_ptr) {
# endif
# ifdef __DEBUG_ENABLED
	ffly_mem_free_c++;
	mdl_u8_t *mem_ptr = ((mdl_u8_t*)__mem_ptr)-sizeof(mdl_uint_t);
	ffly_mem_free_bc += *(mdl_uint_t*)mem_ptr;
	free((void*)mem_ptr);
# else
	free(__mem_ptr);
# endif
}
