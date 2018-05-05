# include "draw.h"
# include "pipe.h"
# include "../dep/bcopy.h"
ff_err_t ffly_pixdraw(ff_uint_t __x, ff_uint_t __y, ff_byte_t *__dst, ff_uint_t __dstw, ff_byte_t *__pixels, ff_uint_t __width, ff_uint_t __height) {
	return ffly_grp_inject(&__ffly_grp__, ffly_grj_pixdraw(__x, __y, __dst, __dstw, __pixels, __width, __height));
}

ff_err_t __ffly_pixdraw(ff_uint_t __x, ff_uint_t __y, ff_byte_t *__dst, ff_uint_t __dstw, ff_byte_t *__pixels, ff_uint_t __width, ff_uint_t __height) {
	ff_uint_t x, y;

	y = __y;
	while(y != __y+__height) {
		x = __x;
		while(x != __x+__width) {
			ffly_bcopy(__dst+(((x+__x)+((y+__y)*__dstw))*4), __pixels+((x+(y*__width))*4), 4);
			x++;
		}
		y++;
	}

}
