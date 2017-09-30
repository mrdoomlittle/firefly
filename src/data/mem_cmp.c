# include "mem_cmp.h"
ffly_byte_t ffly_mem_cmp(void const *__p1, void const *__p2, mdl_uint_t __bc) {
	mdl_u8_t *itr = (mdl_u8_t*)__p1;
	while(itr != (mdl_u8_t*)__p1+__bc) {
		mdl_uint_t off = itr-(mdl_u8_t*)__p1;
		mdl_uint_t bytes_left = __bc-off;
		if (bytes_left >> 3 > 0) {
			if (*(mdl_u64_t*)itr != *(mdl_u64_t*)((mdl_u8_t*)__p2+off)) break;
			itr += sizeof(mdl_u64_t);
		} else if (bytes_left >> 2 > 0) {
			if (*(mdl_u32_t*)itr != *(mdl_u32_t*)((mdl_u8_t*)__p2+off)) break;
			itr += sizeof(mdl_u32_t);
		} else if (bytes_left >> 1 > 0) {
			if (*(mdl_u16_t*)itr != *(mdl_u16_t*)((mdl_u8_t*)__p2+off)) break;
			itr += sizeof(mdl_u16_t);
		} else {
			if (*(mdl_u8_t*)itr != *((mdl_u8_t*)__p2+off)) break;
			itr++;
		}
	}

	if (itr != (mdl_u8_t*)(__p1+__bc)) return 0;
	return 1;
}
