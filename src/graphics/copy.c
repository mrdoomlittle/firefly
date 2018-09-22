# include "copy.h"
# include "pipe.h"
# include "../system/errno.h"
# include "../dep/mem_cpy.h"
# include "../bron/pixel.h"
ff_err_t ffly_pixcopy(ff_u32_t __x, ff_u32_t __y, ff_u32_t __width, ff_u32_t __height) {
	return ffly_grp_inject(&__ffly_grp__, ffly_grj_pixcopy(__x, __y, __width, __height));
}

ff_err_t __ffly_pixcopy(ff_u32_t __x, ff_u32_t __y, ff_u32_t __width, ff_u32_t __height) {
	return FFLY_SUCCESS;
}
