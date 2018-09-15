# include "tex.h"
# include "context.h"
# include "system/io.h"
ff_u16_t ffly_g_tex(struct ffly_tex *__tex) {
	ff_u16_t tex;

	struct ff_context *ctx;
	ctx = G_CONTEXT;

	ctx->driver.tex(__tex, tex = ctx->stack);
	ctx->stack+=ctx->driver.tex_struc_sz;
	return tex;
}
