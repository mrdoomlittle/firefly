# include "mem_alloc.h"
# ifdef __DEBUG_ENABLED
size_t ffly_mem_alloc_bc;
size_t ffly_mem_alloc_c;
# endif

# ifdef __WITH_MEM_TRACKER
void *ffly_mem_alloc(size_t __bc, mdl_u8_t __track_bypass) {
# else
void *ffly_mem_alloc(size_t __bc) {
# endif /*__WITH_MEM_TACKER*/
	void *mem_ptr;
# ifdef __DEBUG_ENABLED
	mem_ptr = malloc(__bc + sizeof(size_t));
	*((size_t *)mem_ptr) = __bc;
	ffly_mem_alloc_bc += __bc;
	ffly_mem_alloc_c++;
	mem_ptr += sizeof(size_t);
# else
	mem_ptr = malloc(__bc);
# endif
# ifdef __WITH_MEM_TRACKER
	if (!__track_bypass)
		ffly_mem_track_alloc(&__ffly_mem_track__, mem_ptr);
# endif
	return mem_ptr;
}
