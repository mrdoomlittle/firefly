# include "mem_cmp.h"
# include "../types/off_t.h"
# include "../system/io.h"
mdl_i8_t ffly_mem_cmp(void const *__p0, void const *__p1, mdl_uint_t __bc) {
	mdl_u8_t *p = (mdl_u8_t*)__p0;
	while(p != (mdl_u8_t*)__p0+__bc) {
		ffly_off_t off = p-(mdl_u8_t*)__p0;
		mdl_uint_t left = __bc-off;
		if (left>>3 > 0) {
			if (*(mdl_u64_t*)p != *(mdl_u64_t*)((mdl_u8_t*)__p1+off)) break;
			p+= sizeof(mdl_u64_t);
		} else if (left>>2 > 0) {
			if (*(mdl_u32_t*)p != *(mdl_u32_t*)((mdl_u8_t*)__p1+off)) break;
			p+= sizeof(mdl_u32_t);
		} else if (left>>1 > 0) {
			if (*(mdl_u16_t*)p != *(mdl_u16_t*)((mdl_u8_t*)__p1+off)) break;
			p+= sizeof(mdl_u16_t);
		} else {
			if (*(mdl_u8_t*)p != *((mdl_u8_t*)__p1+off)) break;
			p++;
		}
	}

	if (p != (mdl_u8_t*)(__p0+__bc)) return -1;
	return 0;
}
