# include "fill.h"
# include "pipe.h"
ff_err_t ffly_pixfill(ff_byte_t *__dst, ff_uint_t __npix, ffly_colour_t __colour) {
	return ffly_grp_inject(&__ffly_grp__, ffly_grj_pixfill(__dst, __npix, __colour));
}

ff_err_t __ffly_pixfill(ff_byte_t *__dst, ff_uint_t __npix, ffly_colour_t __colour) {

}
