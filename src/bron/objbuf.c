# include "objbuf.h"
# include "context.h"
# include "plate.h"
ff_u32_t static
_bron_objbuf_new(ff_u32_t __size) {
	ff_u32_t buf;
	struct bron_context *ctx;
	ctx = BRON_CONTEXT;
	buf = bron_plate_alloc();

	ctx->driver.objbuf_new(buf, __size);
	return buf;
}

void static
_bron_objbuf_destroy(ff_u32_t __buf) {
	BRON_CONTEXT->driver.objbuf_destroy(__buf);
	bron_plate_free(__buf);
}

void static
_bron_objbuf_map(ff_u32_t __buf) {
	BRON_CONTEXT->driver.objbuf_map(__buf);
}

void static
_bron_objbuf_unmap(ff_u32_t __buf) {
	BRON_CONTEXT->driver.objbuf_unmap(__buf);
}

void static
_bron_objbuf_write(ff_u32_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__src) {
	BRON_CONTEXT->driver.objbuf_write(__buf, __offset, __size, __src);
}

void static
_bron_objbuf_read(ff_u32_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__dst) {
	BRON_CONTEXT->driver.objbuf_read(__buf, __offset, __size, __dst);
}

ff_u32_t ffly_bron_objbuf_new(ff_u32_t __size) {
	return _bron_objbuf_new(__size);
}

void ffly_bron_objbuf_destroy(ff_u32_t __buf) {
	_bron_objbuf_destroy(__buf);
}

void ffly_bron_objbuf_map(ff_u32_t __buf) {
	_bron_objbuf_map(__buf);
}

void ffly_bron_objbuf_unmap(ff_u32_t __buf) {
	_bron_objbuf_unmap(__buf);
}

void ffly_bron_objbuf_write(ff_u32_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__src) {
	_bron_objbuf_write(__buf, __offset, __size, __src);
}

void ffly_bron_objbuf_read(ff_u32_t __buf, ff_u32_t __offset, ff_u32_t __size, void *__dst) {
	_bron_objbuf_read(__buf, __offset, __size, __dst);
}
