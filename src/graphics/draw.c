# include "draw.h"
# include "pipe.h"
# include "../dep/bcopy.h"
# include "../context.h"
# include "../bron/pixel.h"
# include "../system/errno.h"
# include "../tile.h"
void ffly_tdraw(ffly_tilep __t, ff_u32_t __x, ff_u32_t __y) {
	ffly_grp_inject(&__ffly_grp__, ffly_grj_tdraw(__t, __x, __y));
}

void __ffly_tdraw(ffly_tilep __t, ff_u32_t __x, ff_u32_t __y) {
	ffly_tile_draw(__t, __x, __y);
}

ff_err_t ffly_pixdraw(ff_u32_t __x, ff_u32_t __y, void *__src,
	ff_u32_t __width, ff_u32_t __height)
{
	return ffly_grp_inject(&__ffly_grp__, ffly_grj_pixdraw(__x, __y, __src, __width, __height));
}

ff_err_t __ffly_pixdraw(ff_u32_t __x, ff_u32_t __y, void *__src,
	ff_u32_t __width, ff_u32_t __height)
{
	bron_pixdraw(__x, __y, __src, __width, __height);
	return FFLY_SUCCESS;
}
