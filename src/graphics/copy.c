# include "copy.h"
# include "pipe.h"
ff_err_t ffly_pixcopy(ff_byte_t *__dst, ff_byte_t *__src, ff_uint_t __npix) {
	return ffly_grp_inject(&__ffly_grp__, ffly_grj_pixcopy(__dst, __src, __npix));
}

ff_err_t __ffly_pixcopy(ff_byte_t *__dst, ff_byte_t *__src, ff_uint_t __npix) {

}
