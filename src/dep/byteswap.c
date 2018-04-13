# include "byteswap.h"
ff_u16_t bswp_16(ff_u16_t __v) {
	register ff_u16_t x = __v;
	register ff_u16_t ret = 0;
	__asm__("rorw $8, %w0" : "=r"(ret) : "0"(x));
	return ret;
}
