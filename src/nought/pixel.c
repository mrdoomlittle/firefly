# include "pixel.h"
# include "shit.h"
void nt_setpix(ff_u8_t *__ntc, ff_u8_t *__dst) {
	if (is_bit(nt_bits, NT_BLEND)) {
		*__dst = ((*__dst)+*__ntc)>>1;
		__dst[1] = (__dst[1]+__ntc[1])>>1;
		__dst[2] = (__dst[2]+__ntc[2])>>1;
		__dst[3] = (__dst[3]+__ntc[3])>>1;
	} else {
		ff_u16_t da, sa;
		da = 255-__ntc[3];
		sa = __ntc[3];
		*__dst = ((da*(ff_u16_t)*__dst)+(sa*(ff_u16_t)*__ntc))>>8;
		__dst[1] = ((da*(ff_u16_t)__dst[1])+(sa*(ff_u16_t)__ntc[1]))>>8;
		__dst[2] = ((da*(ff_u16_t)__dst[2])+(sa*(ff_u16_t)__ntc[2]))>>8;
		__dst[3] = (da+sa)>>1;
	}
}

