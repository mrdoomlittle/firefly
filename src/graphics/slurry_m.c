# include "slurry_m.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "mare.h"
void static
_s_display_open(void *__ctx) {
	struct ffly_slurry_ctx *ctx;
	ctx = (struct ffly_slurry_ctx*)__ctx;
	ctx->conn = s_open();
	s_connect(ctx->conn, 10198, "127.0.0.1");
	ctx->d = s_display_open(ctx->conn);
}

void static
_s_display_close(void *__ctx) {
	struct ffly_slurry_ctx *ctx;
	ctx = (struct ffly_slurry_ctx*)__ctx;
	s_display_close(ctx->conn, ctx->d);
	s_disconnect(ctx->conn);
	s_close(ctx->conn);
}

void static
_s_init(void *__ctx) {
	struct ffly_slurry_ctx *ctx;
	ctx = (struct ffly_slurry_ctx*)__ctx;
}

void static
_s_de_init(void *__ctx) {
	struct ffly_slurry_ctx *ctx;
	ctx = (struct ffly_slurry_ctx*)__ctx;
}

void static
_s_cleanup(void *__ctx) {
	struct ffly_slurry_ctx *ctx;
	ctx = (struct ffly_slurry_ctx*)__ctx;
}

# include "../dep/str_len.h"
void static
_s_window_creat(void *__ctx, ff_uint_t __width, ff_uint_t __height, char const *__title) {
	struct ffly_slurry_ctx *ctx;
	ctx = (struct ffly_slurry_ctx*)__ctx;
	ctx->frame_buff = (ff_u8_t*)__ffly_mem_alloc(__width*__height*4);
	ctx->width = __width;
	ctx->height = __height;
	ctx->w = s_window_new(ctx->conn, ctx->d);
	ctx->title =  s_rtn(ffly_str_len(__title)+1);
	s_write(ctx->conn, ctx->title, ffly_str_len(__title)+1, __title);
	s_window_init(ctx->conn, ctx->w, __width, __height, ctx->title);
}

void static
_s_window_destroy(void *__ctx) {
	struct ffly_slurry_ctx *ctx;
	ctx = (struct ffly_slurry_ctx*)__ctx;
	s_window_destroy(ctx->conn, ctx->w);
	__ffly_mem_free(ctx->frame_buff);
}

void static
_s_window_display(void *__ctx) {
	struct ffly_slurry_ctx *ctx;
	ctx = (struct ffly_slurry_ctx*)__ctx;
	s_window_frame(ctx->conn, ctx->w, ctx->frame_buff, ctx->width, ctx->height);
	s_window_display(ctx->conn, ctx->w);
}

void static
_s_get(ff_u8_t __what, long long __arg, void *__ctx) {
	struct ffly_slurry_ctx *ctx;
	ctx = (struct ffly_slurry_ctx*)__ctx;
	switch(__what) {
		case _MARE_GET_FRAME_BUFF:
			*(void**)__arg = ctx->frame_buff;
		break;
		case _MARE_GET_WIDTH:
			*(ff_uint_t*)__arg = ctx->width;
		break;
		case _MARE_GET_HEIGHT:
			*(ff_uint_t*)__arg = ctx->height;
		break;
	}
}

static struct ffly_slurry_ctx slur_ctx;
void ffly_mare_slurry(ffly_mctxp __m) {
	__m->driver.display_open = _s_display_open;
	__m->driver.display_close = _s_display_close;
	__m->driver.init = _s_init;
	__m->driver.de_init = _s_de_init;
	__m->driver.cleanup = _s_cleanup;
	__m->driver.window_creat = _s_window_creat;
	__m->driver.window_destroy = _s_window_destroy;
	__m->driver.window_display = _s_window_display;
	__m->driver.get = _s_get;
	__m->context = (void*)&slur_ctx;
}
