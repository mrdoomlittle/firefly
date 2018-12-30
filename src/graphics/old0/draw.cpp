# include "draw.h"
extern "C" {
ffly_err_t ffly_pixeldraw(mdl_uint_t __x, mdl_uint_t __y, ffly_byte_t *__dst, mdl_uint_t __dstw, ffly_byte_t *__pixels, mdl_uint_t __width, mdl_uint_t __height) {
    return mdl::firefly::graphics::pixeldraw(__x, __y, __dst, __dstw, __pixels, __width, __height);
}
ffly_err_t ffly_pixdraw(mdl_uint_t __x, mdl_uint_t __y, ffly_byte_t *__dst, mdl_uint_t __dstw, ffly_byte_t *__pixels, mdl_uint_t __width, mdl_uint_t __height) {
	return mdl::firefly::graphics::gpu_pixdraw(__x, __y, __dst, __dstw, __pixels, __width, __height);
}
}
