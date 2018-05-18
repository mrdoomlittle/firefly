# include "mm.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../system/io.h"
void* ff_resin_mmap(ff_uint_t __size) {
	ffly_printf("resin, mmap: %u\n", __size);
	return __ffly_mem_alloc(__size);
}

void ff_resin_munmap(void *__p) {	
	__ffly_mem_free(__p);
}
