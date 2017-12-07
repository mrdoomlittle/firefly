# include "copy.h"
extern "C" {
ffly_err_t ffly_pixcopy(ffly_byte_t *__dst, ffly_byte_t *__src, mdl_uint_t __nopix) {
	return mdl::firefly::graphics::gpu_pixcopy(__dst, __src, __nopix);
}
}
