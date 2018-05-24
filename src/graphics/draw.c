# include "draw.h"
# include "pipe.h"
# include "../dep/bcopy.h"
# include "../context.h"
# include "../pixel.h"
ff_err_t ffly_pixdraw(ff_uint_t __x, ff_uint_t __y, ff_byte_t *__dst, ff_uint_t __dstw, ff_byte_t *__pixels, ff_uint_t __width, ff_uint_t __height) {
	return ffly_grp_inject(&__ffly_grp__, ffly_grj_pixdraw(__x, __y, __dst, __dstw, __pixels, __width, __height));
}

ff_err_t __ffly_pixdraw(ff_uint_t __x, ff_uint_t __y, ff_byte_t *__dst, ff_uint_t __dstw, ff_byte_t *__pixels, ff_uint_t __width, ff_uint_t __height) {
	ff_uint_t x, y;

	ff_u8_t blocksize = PIXEL_BLOCK;

	y = __y;
	while(y != __y+__height) {
		x = __x;
		while(x != __x+__width) {
			ffly_bcopy(__dst+((x+(y*__dstw))*blocksize), __pixels+(((x-__x)+((y-__y)*__width))*blocksize), blocksize);
			x++;
		}
		y++;
	}

}
