# include "mem_realloc.h"
void *ffly_mem_realloc(void *__mptr, size_t __nbc) {
	__mptr -= sizeof(size_t);
	*((size_t *)__mptr) = __nbc;
	return realloc(__mptr, __nbc + sizeof(size_t)) + sizeof(size_t);
}
