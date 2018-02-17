# include <mdlint.h>
# include "../system/io.h"
void ffly_printbin(mdl_u64_t __val, mdl_u8_t __n) {
	mdl_u8_t off = 0;
	while(off != __n) {
		ffly_printf("%u", __val>>((__n-1)-off)&0x1);
	}
	ffly_printf("\n");
}
