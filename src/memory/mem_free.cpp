# include "mem_free.h"
# ifdef __DEBUG_ENABLED
std::size_t mdl::firefly::memory::free_bc;
std::size_t mdl::firefly::memory::free_c;
# endif

# ifdef __WITH_MEM_TRACKER
void mdl::firefly::memory::mem_free(void *__mem_ptr, bool __track_bypass) {
	if (!__track_bypass)
		ffly_mem_track_free(&__ffly_mem_track__, __mem_ptr, 0);
# else
void mdl::firefly::memory::mem_free(void *__mem_ptr) {
# endif
# ifdef __DEBUG_ENABLED
	u8_t *mem_ptr = ((u8_t*)__mem_ptr) - sizeof(std::size_t);
	free_c ++;
	free_bc += *(std::size_t *)mem_ptr;
	std::free((void*)mem_ptr);
# else
	std::free(__mem_ptr);
# endif
}
