# include "tri.h"
# include "tex.h"
# include "context.h"
ff_u16_t static
_bron_tri2(struct bron_tri2 *__tri, ff_u16_t __tex, ff_u32_t __x, ff_u32_t __y) {
	ff_u16_t tri;
	struct bron_context *ctx;
	ctx = BRON_CONTEXT;

	ctx->driver.tri2(__tri, tri = ctx->stack);
	ctx->driver.raster_tri2(tri, __tex, __x, __y);

	ctx->stack+=ctx->driver.tri2_struc_sz;
	return tri;
}

ff_u16_t ffly_bron_tri2(struct bron_tri2 *__tri, ff_u16_t __tex, ff_u32_t __x, ff_u32_t __y) {
	return _bron_tri2(__tri, __tex, __x, __y);
}
