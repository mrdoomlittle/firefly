# include <eint_t.h>
void ffly_byte_ncpy(void *__dest, void *__src, mdl_uint_t __n) {
	mdl_u8_t *src_itr = (mdl_u8_t*)__src;
	while(src_itr != (mdl_u8_t*)__src+__n) {
		mdl_uint_t off = src_itr-(mdl_u8_t*)__src;
		mdl_uint_t bytes_left = __n-off;
		if (bytes_left >> 3 > 0) {
			*((mdl_u64_t*)((mdl_u8_t*)__dest+off)) = *(mdl_u64_t*)src_itr;
			src_itr += sizeof(mdl_u64_t);
		} else if (bytes_left >> 2 > 0) {
			*((mdl_u32_t*)((mdl_u8_t*)__dest+off)) = *(mdl_u32_t*)src_itr;
			src_itr += sizeof(mdl_u32_t);
		} else if (bytes_left >> 1 > 0) {
			*((mdl_u16_t*)((mdl_u8_t*)__dest+off)) = *(mdl_u16_t*)src_itr;
			src_itr += sizeof(mdl_u16_t);
		} else
			*((mdl_u8_t*)__dest+off) = *(src_itr++);
	}
}
