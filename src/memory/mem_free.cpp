# include "mem_free.h"
# ifdef __DEBUG_ENABLED
std::size_t mdl::firefly::memory::free_bc;
std::size_t mdl::firefly::memory::free_c;
# endif

# ifdef __WITH_MEM_TRACKER
void mdl::firefly::memory::mem_free(void *__mptr, bool __track_bypass) {
	if (!__track_bypass)
		ffly_mem_track_free(&__ffly_mem_track__, __mptr, 0);
# else
void mdl::firefly::memory::mem_free(void *__mptr) {
# endif

# ifdef __DEBUG_ENABLED
	free_c ++;
	__mptr -= sizeof(std::size_t);
	free_bc += *(std::size_t *)__mptr;
# endif
	std::free(__mptr);
}
