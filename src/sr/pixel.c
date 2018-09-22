# include "pixel.h"
# include "shit.h"
void sr_setpix(ff_u8_t *__src, ff_u8_t *__dst) {
	if (is_bit(sr_bits, SR_BLEND)) {
		*__dst = ((*__dst)+*__src)>>1;
		__dst[1] = (__dst[1]+__src[1])>>1;
		__dst[2] = (__dst[2]+__src[2])>>1;
		__dst[3] = (__dst[3]+__src[3])>>1;
	} else {
		*(ff_u32_t*)__dst = *(ff_u32_t*)__src;
	}
}

