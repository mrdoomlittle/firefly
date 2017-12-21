# include "mem_cmp.h"
# include "../types/off_t.h"
mdl_i8_t ffly_mem_cmp(void const *__p1, void const *__p2, mdl_uint_t __bc) {
	mdl_u8_t *itr = (mdl_u8_t*)__p1;
	while(itr != (mdl_u8_t*)__p1+__bc) {
		ffly_off_t off = itr-(mdl_u8_t*)__p1;
		mdl_uint_t left = __bc-off;
		if (left>>3 > 0) {
			if (*(mdl_u64_t*)itr != *(mdl_u64_t*)((mdl_u8_t*)__p2+off)) break;
			itr+= sizeof(mdl_u64_t);
		} else if (left>>2 > 0) {
			if (*(mdl_u32_t*)itr != *(mdl_u32_t*)((mdl_u8_t*)__p2+off)) break;
			itr+= sizeof(mdl_u32_t);
		} else if (left>>1 > 0) {
			if (*(mdl_u16_t*)itr != *(mdl_u16_t*)((mdl_u8_t*)__p2+off)) break;
			itr+= sizeof(mdl_u16_t);
		} else {
			if (*(mdl_u8_t*)itr != *((mdl_u8_t*)__p2+off)) break;
			itr++;
		}
	}

	if (itr != (mdl_u8_t*)(__p1+__bc)) return 0;
	return 1;
}
