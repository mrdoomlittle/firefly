# include "pixel.h"
# include "format.h"
void ffly_pix_set(void *__colour, void *__pixels, ff_uint_t __n, ff_u8_t __format) {
	if (__format == _FF_FORMAT_RGBA) {
		ff_uint_t i;
		ff_u8_t *d, *s;
		d = (ff_u8_t*)__pixels;
		s = (ff_u8_t*)__colour;
		ff_uint_t off;
		i = 0;
		while(i != __n) {
			off = i<<2;
			*(ff_u32_t*)(d+off) = *(ff_u32_t*)s;
			i++;
		}
	}
}
