# include "mm.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../system/io.h"
/*
	for now, later we will have a large stack of memory to work with but limited,
	memory alloc and free wont be done thru allocr but hear for speed,
	also only things useing allocr would for large allocs over X size but only if 
	prog has primission to do so.
*/
void* ff_resin_mmap(ff_uint_t __size) {
	ffly_printf("resin, mmap: %u\n", __size);
	return __ffly_mem_alloc(__size);
}

void ff_resin_munmap(void *__p) {	
	__ffly_mem_free(__p);
}
