# include "mem_alloc.h"
# include "../system/errno.h"
# include "../system/io.h"
# ifdef __DEBUG_ENABLED
ffly_atomic_uint_t ffly_mem_alloc_bc = 0;
ffly_atomic_uint_t ffly_mem_alloc_c = 0;
# endif

# ifdef __MEM_AGENT
void* ffly_mem_alloc(mdl_uint_t __bc, mdl_u8_t __agent_ignore) {
# else
void* ffly_mem_alloc(mdl_uint_t __bc) {
# endif /*__MEM_AGENT*/
	mdl_u8_t *p;
# ifdef __DEBUG_ENABLED
	if ((p = (mdl_u8_t*)malloc(__bc+sizeof(mdl_uint_t))) == NULL) {
		ffly_printf(stderr, "mem_alloc: failed to allocate memory.\n");
		return NULL;
	}

	*((mdl_uint_t*)p) = __bc;
	ffly_atomic_add(&ffly_mem_alloc_bc, __bc);
	ffly_atomic_incr(&ffly_mem_alloc_c);
	p += sizeof(mdl_uint_t);
# else
	p = (mdl_u8_t*)malloc(__bc);
# endif
# ifdef __MEM_AGENT
	if (!__agent_ignore) {
		if (ffly_mem_agent_alloc(&__ffly_mem_agent__, (void*)p) != FFLY_SUCCESS) {
# ifdef __DEBUG_ENABLED
			ffly_printf(stderr, "mem_alloc: mem agent failure.\n");
# endif
			free((void*)(p+sizeof(mdl_uint_t)));
			return NULL;
		}
	}
# endif
	return (void*)p;
}
