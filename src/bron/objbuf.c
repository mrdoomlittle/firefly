# include "objbuf.h"
# include "context.h"
ff_u16_t static
_bron_objbuf_new(ff_u32_t __size) {
	ff_u16_t buf;
	struct bron_context *ctx;
	ctx = BRON_CONTEXT;
	buf = ctx->stack;
	ctx->stack+=8;

	ctx->driver.objbuf_new(buf, __size);
	return buf;
}

void static
_bron_objbuf_destroy(ff_u16_t __buf) {
	BRON_CONTEXT->driver.objbuf_destroy(__buf);
}

void static
_bron_objbuf_map(ff_u16_t __buf) {
	BRON_CONTEXT->driver.objbuf_map(__buf);
}

void static
_bron_objbuf_unmap(ff_u16_t __buf) {
	BRON_CONTEXT->driver.objbuf_unmap(__buf);
}

void static
_bron_objbuf_write(ff_u16_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__src) {
	BRON_CONTEXT->driver.objbuf_write(__buf, __offset, __size, __src);
}

void static
_bron_objbuf_read(ff_u16_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__dst) {
	BRON_CONTEXT->driver.objbuf_read(__buf, __offset, __size, __dst);
}

ff_u16_t ffly_bron_objbuf_new(ff_u32_t __size) {
	return _bron_objbuf_new(__size);
}

void ffly_bron_objbuf_destroy(ff_u16_t __buf) {
	_bron_objbuf_destroy(__buf);
}

void ffly_bron_objbuf_map(ff_u16_t __buf) {
	_bron_objbuf_map(__buf);
}

void ffly_bron_objbuf_unmap(ff_u16_t __buf) {
	_bron_objbuf_unmap(__buf);
}

void ffly_bron_objbuf_write(ff_u16_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__src) {
	_bron_objbuf_write(__buf, __offset, __size, __src);
}

void ffly_bron_objbuf_read(ff_u16_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__dst) {
	_bron_objbuf_read(__buf, __offset, __size, __dst);
}
