# include "draw.h"
# include "pipe.h"
# include "../dep/bcopy.h"
# include "../context.h"
# include "../pixel.h"
# include "../system/errno.h"

ff_err_t ffly_pixdraw(ff_u32_t __x, ff_u32_t __y, ffly_palletp __src,
	ff_u32_t __width, ff_u32_t __height)
{
	return ffly_grp_inject(&__ffly_grp__, ffly_grj_pixdraw(__x, __y, __src, __width, __height));
}

ff_err_t __ffly_pixdraw(ff_u32_t __x, ff_u32_t __y, ffly_palletp __src,
	ff_u32_t __width, ff_u32_t __height)
{
	ff_uint_t x, y;
	return FFLY_SUCCESS;
}
