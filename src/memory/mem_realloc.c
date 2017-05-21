# include "mem_realloc.h"
void *ffly_mem_realloc(void *__mem_ptr, size_t __nbc) {
# ifdef __DEBUG_ENABLED
	mdl_u8_t *mem_ptr = ((mdl_u8_t*)__mem_ptr) - sizeof(size_t);
	size_t mem_size = *((size_t*)mem_ptr);

	if (__nbc > mem_size)
		ffly_mem_alloc_bc += __nbc - mem_size;
	else if (__nbc < mem_size)
		ffly_mem_alloc_bc -= mem_size - __nbc;

	*((size_t*)mem_ptr) = __nbc;
	mem_ptr = ((mdl_u8_t*)realloc((void*)mem_ptr, __nbc + sizeof(size_t))) + sizeof(size_t);
	return (void*)mem_ptr;
# else
	return realloc(__mem_ptr, __nbc);
# endif
}
