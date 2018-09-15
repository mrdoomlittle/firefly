# include "mo.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
ffly_mop ffly_mo_new(void) {
	ffly_mop mo;

	mo = (ffly_mop)__ffly_mem_alloc(sizeof(struct ffly_mo));
	mo->map = NULL;
	return mo;
}

void ffly_mo_destroy(ffly_mop __mo) {
	__ffly_mem_free(__mo);
}

void ffly_mo_map(ffly_mop __mo, ff_uint_t __size) {
	__mo->map = __ffly_mem_alloc(__size);
}

void ffly_mo_unmap(ffly_mop __mo) {
	__ffly_mem_free(__mo->map);
}

# include "dep/mem_cpy.h"
ffly_mop ffly_mo_read(ffly_mop __mo, void *__buf, ff_uint_t __size, ff_u32_t __offset) {
	ffly_mem_cpy(__buf, ((ff_u8_t*)__mo->map)+__offset, __size);
}

ffly_mop ffly_mo_write(ffly_mop __mo, void *__buf, ff_uint_t __size, ff_u32_t __offset) {
	ffly_mem_cpy(((ff_u8_t*)__mo->map)+__offset,  __buf, __size);
}
