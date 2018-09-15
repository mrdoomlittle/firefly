# include "tri.h"
# include "tex.h"
# include "context.h"
ff_u16_t ffly_g_tri2(struct ffly_tri2 *__tri, ff_u16_t __tex) {
	ff_u16_t tri;
	struct ff_context *ctx;
	ctx = G_CONTEXT;

	ctx->driver.tri2(__tri, tri = ctx->stack);
	ctx->driver.raster_tri2(tri, __tex);

	ctx->stack+=ctx->driver.tri2_struc_sz;
	return tri;
}
