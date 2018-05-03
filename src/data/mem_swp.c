# include "mem_swp.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/mem_cpy.h"
void ffly_mem_swp(void *__p1, void *__p2, ff_uint_t __bc) {
	void *tmp;
	ff_u8_t fast[sizeof(ff_u64_t)];
	ff_u8_t alloced;
	if (__bc > sizeof(ff_u64_t)) {
		tmp = __ffly_mem_alloc(__bc);
		alloced = 1;
	} else {
		tmp = (void*)fast;
		alloced = 0;
	}

	ffly_mem_cpy(tmp, __p1, __bc);
	ffly_mem_cpy(__p1, __p2, __bc);
	ffly_mem_cpy(__p2, tmp, __bc);

	if (alloced)
		__ffly_mem_free(tmp);
}
