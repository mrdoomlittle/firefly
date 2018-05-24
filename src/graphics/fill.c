# include "fill.h"
# include "pipe.h"
ff_err_t ffly_pixfill(ff_byte_t *__dst, ff_uint_t __npix, ffly_colour_t __colour) {
	return ffly_grp_inject(&__ffly_grp__, ffly_grj_pixfill(__dst, __npix, __colour));
}

ff_err_t __ffly_pixfill(ff_byte_t *__dst, ff_uint_t __npix, ffly_colour_t __colour) {
	ff_u8_t *p = __dst;
	ff_u8_t *end = p+(__npix*4);
	while(p != end) {
		*(p++) = __colour.r;
		*(p++) = __colour.g;
		*(p++) = __colour.b;
		*(p++) = __colour.a;
	}
}
