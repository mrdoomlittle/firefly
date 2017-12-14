# include "mem_set.h"
void* ffly_mem_set(void *__dst, mdl_u8_t __val, mdl_uint_t __bc) {
	mdl_u8_t *itr = (mdl_u8_t*)__dst;
	while(itr != (mdl_u8_t*)__dst+__bc)
		*(itr++) = __val;
	return __dst;
}
