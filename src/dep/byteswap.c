# include "byteswap.h"
mdl_u16_t bswp_16(mdl_u16_t __v) {
	register mdl_u16_t x = __v;
	register mdl_u16_t ret;
	__asm__("rorw $8, %w0" : "=r"(ret) : "0"(x));
	return ret;
}
