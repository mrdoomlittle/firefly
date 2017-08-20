# include "mem_alloc.h"
# include "../system/io.h"
# ifdef __DEBUG_ENABLED
mdl_uint_t ffly_mem_alloc_bc;
mdl_uint_t ffly_mem_alloc_c;
# endif

# ifdef __WITH_MEM_TRACKER
void* ffly_mem_alloc(mdl_uint_t __bc, mdl_u8_t __track_bypass) {
# else
void* ffly_mem_alloc(mdl_uint_t __bc) {
# endif /*__WITH_MEM_TACKER*/
	mdl_u8_t *mem_ptr;
# ifdef __DEBUG_ENABLED
	if ((mem_ptr = (mdl_u8_t*)malloc(__bc+sizeof(mdl_uint_t))) == NULL) {
		ffly_printf(stderr, "mem_alloc: failed to allocate memory.\n");
		return NULL;
	}

	*((mdl_uint_t*)mem_ptr) = __bc;
	ffly_mem_alloc_bc += __bc;
	ffly_mem_alloc_c++;
	mem_ptr += sizeof(mdl_uint_t);
# else
	mem_ptr = (mdl_u8_t*)malloc(__bc);
# endif
# ifdef __WITH_MEM_TRACKER
	if (!__track_bypass)
		ffly_mem_track_alloc(&__ffly_mem_track__, (void*)mem_ptr);
# endif
	return (void*)mem_ptr;
}
