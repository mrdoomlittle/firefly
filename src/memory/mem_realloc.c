# include "mem_realloc.h"
# include "../system/io.h"
# ifdef __WITH_MEM_TRACKER
void* ffly_mem_realloc(void *__mem_ptr, mdl_uint_t __nbc, ffly_bool_t __track_bypass) {
# else
void* ffly_mem_realloc(void *__mem_ptr, mdl_uint_t __nbc);
# endif
	mdl_u8_t *mem_ptr;
# ifdef __DEBUG_ENABLED
	mem_ptr = ((mdl_u8_t*)__mem_ptr)-sizeof(mdl_uint_t);
	mdl_uint_t mem_size = *((mdl_uint_t*)mem_ptr);

	if ((mem_ptr = (mdl_u8_t*)realloc(mem_ptr, __nbc+sizeof(mdl_uint_t))) == NULL) {
		ffly_printf(stderr, "mem_realloc: failed to reallocate memory.\n");
		return NULL;
	}

	if (__nbc > mem_size)
		ffly_mem_alloc_bc += __nbc-mem_size;
	else if (__nbc < mem_size)
		ffly_mem_alloc_bc -= mem_size-__nbc;

	*((mdl_uint_t*)mem_ptr) = __nbc;
	mem_ptr += sizeof(mdl_uint_t);
# else
	mem_ptr = (mdl_u8_t*)realloc(__mem_ptr, __nbc);
# endif

# ifdef __WITH_MEM_TRACKER
	if (!__track_bypass)
		ffly_mem_track_update(&__ffly_mem_track__, __mem_ptr, mem_ptr);
# endif
	return mem_ptr;
}
