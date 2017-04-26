# include "mem_alloc.h"
# ifdef __DEBUG_ENABLED
std::size_t mdl::firefly::memory::alloc_bc;
std::size_t mdl::firefly::memory::alloc_c;
# endif
# ifdef __WITH_MEM_TRACKER
void* mdl::firefly::memory::mem_alloc(std::size_t __bc, bool __track_bypass) {
# else
void* mdl::firefly::memory::mem_alloc(std::size_t __bc) {
# endif
	void *mptr;
# ifdef __DEBUG_ENABLED
	mptr = malloc(__bc + sizeof(std::size_t));
	*((std::size_t *)mptr) = __bc;
	alloc_bc += __bc;
	alloc_c ++;

	mptr += sizeof(std::size_t);
# else
	mptr = malloc(__bc);
# endif
# ifdef __WITH_MEM_TRACKER
	if (!__track_bypass)
		ffly_mem_track_alloc(&__ffly_mem_track__, mptr);
# endif
	return mptr;
}
