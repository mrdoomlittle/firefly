# include "fill.h"
extern "C" {
ffly_err_t ffly_pixfill(ffly_byte_t *__dst, mdl_uint_t __nopix, ffly_colour_t __colour) {
	return mdl::firefly::graphics::gpu_pixfill(__dst, __nopix, __colour);
}
}
