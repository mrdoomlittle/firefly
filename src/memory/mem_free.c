# include "mem_free.h"
# ifdef __DEBUG_ENABLED
size_t ffly_mem_free_bc;
size_t ffly_mem_free_c;
# endif

# ifdef __WITH_MEM_TRACKER
void ffly_mem_free(void *__mptr, mdl_u8_t __track_bypass) {
	if (!__track_bypass)
		ffly_mem_track_free(&__ffly_mem_track__, __mptr, 0);
# else
void ffly_mem_free(void *__mptr) {
# endif
# ifdef __DEBUG_ENABLED
	ffly_mem_free_c ++;
	__mptr -= sizeof(size_t);
	ffly_mem_free_bc += *(size_t *)__mptr;
# endif
	free(__mptr);
}
