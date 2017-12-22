# include "mem_swp.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "mem_cpy.h"
void ffly_mem_swp(void const *__p1, void const *__p2, mdl_uint_t __bc) {
	void *tmp;
	mdl_u8_t fast[sizeof(mdl_u64_t)];
	mdl_u8_t alloced;
	if (__bc > sizeof(mdl_u64_t)) {
		tmp = __ffly_mem_alloc(__bc);
		alloced = 1;
	} else {
		tmp = (void*)fast;
		alloced = 0;
	}

	ffly_mem_cpy((void*)tmp, __p1, __bc);
	ffly_mem_cpy(__p1, __p2, __bc);
	ffly_mem_cpy(__p2, (void*)tmp, __bc);

	if (alloced)
		__ffly_mem_free(tmp);
}
