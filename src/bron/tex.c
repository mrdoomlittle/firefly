# include "tex.h"
# include "context.h"
# include "../system/io.h"
ff_u16_t static
_bron_tex(struct bron_tex *__tex) {
	ff_u16_t tex;

	struct bron_context *ctx;
	ctx = BRON_CONTEXT;

	ctx->driver.tex(__tex, tex = ctx->stack);
	ctx->stack+=ctx->driver.tex_struc_sz;
	return tex;
}

ff_u16_t ffly_bron_tex(struct bron_tex *__tex) {
	return _bron_tex(__tex);
}
