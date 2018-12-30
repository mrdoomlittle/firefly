# include "draw.h"
# include "pipe.h"
# include "../dep/bcopy.h"
# include "../context.h"
# include "../bron/pixel.h"
# include "../system/errno.h"
# include "../tile.h"
# include "../bron/tri.h"
void ffly_tri2(struct ffly_tri2 *__tri, ff_u16_t __tex, ff_u32_t __x, ff_u32_t __y) {
	ffly_grp_inject(&__ffly_grp__, ffly_grj_tri2(__tri, __tex, __x, __y));
}

void ffly_tdraw(ffly_tilep __t, ff_u32_t __x, ff_u32_t __y) {
	ffly_grp_inject(&__ffly_grp__, ffly_grj_tdraw(__t, __x, __y));
}

ff_i8_t __ffly_tdraw(ffly_tilep __t, ff_u32_t __x, ff_u32_t __y) {
	ffly_tile_draw(__t, __x, __y);
	return 0;
}

ff_err_t ffly_pixdraw(ff_u32_t __x, ff_u32_t __y, void *__src,
	ff_u32_t __width, ff_u32_t __height)
{
	return ffly_grp_inject(&__ffly_grp__, ffly_grj_pixdraw(__x, __y, __src, __width, __height));
}


ff_i8_t __ffly_tri2(struct ffly_tri2 *__tri, ff_u16_t __tex, ff_u32_t __x, ff_u32_t __y) {
	struct bron_tri2 tri;

	tri = *(struct bron_tri2*)&__tri->coords;
	ffly_bron_tri2(&tri, __tex, __x, __y);
	return 0;
}

ff_i8_t __ffly_pixdraw(ff_u32_t __x, ff_u32_t __y, void *__src,
	ff_u32_t __width, ff_u32_t __height)
{
	bron_pixdraw(__x, __y, __src, __width, __height);
	return 0;
}
