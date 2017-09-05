# include "mem_cpy.h"
void ffly_mem_cpy(void *__dst, void *__src, mdl_uint_t __bc) {
	mdl_u8_t *src_itr = (mdl_u8_t*)__src;
	while(src_itr != (mdl_u8_t*)__src+__bc) {
		mdl_uint_t off = src_itr-(mdl_u8_t*)__src;
		mdl_uint_t bytes_left = __bc-off;
		if (bytes_left >> 3 > 0) {
			*((mdl_u64_t*)((mdl_u8_t*)__dst+off)) = *(mdl_u64_t*)src_itr;
			src_itr += sizeof(mdl_u64_t);
		} else if (bytes_left >> 2 > 0) {
			*((mdl_u32_t*)((mdl_u8_t*)__dst+off)) = *(mdl_u32_t*)src_itr;
			src_itr += sizeof(mdl_u32_t);
		} else if (bytes_left >> 1 > 0) {
			*((mdl_u16_t*)((mdl_u8_t*)__dst+off)) = *(mdl_u16_t*)src_itr;
			src_itr += sizeof(mdl_u16_t);
		} else
			*((mdl_u8_t*)__dst+off) = *(src_itr++);
	}
}
