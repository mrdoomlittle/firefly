# include "tile.h"
# include "context.h"
ff_u16_t static
_bron_ptile_new(void(*__get)(ff_u8_t, long long, void*), void *__tile) {
	ff_u16_t pt;

	struct bron_context *ctx;
	ctx = BRON_CONTEXT;
	pt = ctx->stack;
	ctx->stack+=2;
	ctx->driver.ptile_new(pt, __get, __tile);
	return pt;
}

void static
_bron_ptile_destroy(ff_u16_t __pt) {
	BRON_CONTEXT->driver.ptile_destroy(__pt);
}

void static
_bron_tdraw(ff_u16_t __t, ff_u32_t __x, ff_u32_t __y) {
	BRON_CONTEXT->driver.tdraw(__t, __x, __y);
}

ff_u16_t ffly_bron_ptile_new(void(*__get)(ff_u8_t, long long, void*), void *__tile) {
	return _bron_ptile_new(__get, __tile);
}

void ffly_bron_ptile_destroy(ff_u16_t __pt) {
	_bron_ptile_destroy(__pt);
}

void ffly_bron_tdraw(ff_u16_t __t, ff_u32_t __x, ff_u32_t __y) {
	_bron_tdraw(__t, __x, __y);
}
