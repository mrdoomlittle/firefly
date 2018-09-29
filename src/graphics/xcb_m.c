# include "xcb_m.h"
# include "../system/io.h"
# include "../data/mem_set.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../data/str_len.h"
# include "mare.h"
void static
xcb_init(void *__ctx) {
	struct ffly_xcb_ctx *ctx;
	ctx = (struct ffly_xcb_ctx*)__ctx;
	ffly_mem_set(ctx, 0, sizeof(struct ffly_xcb_ctx));
}

void static
xcb_de_init(void *__ctx) {
	struct ffly_xcb_ctx *ctx;
	ctx = (struct ffly_xcb_ctx*)__ctx;
}

int static visatt[] = {
	GLX_X_RENDERABLE, True,
	GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
	GLX_RENDER_TYPE, GLX_RGBA_BIT,
	GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
	GLX_RED_SIZE, 8,
	GLX_GREEN_SIZE, 8,
	GLX_BLUE_SIZE, 8,
	GLX_ALPHA_SIZE, 8,
	GLX_DEPTH_SIZE, 24,
	GLX_STENCIL_SIZE, 8,
	GLX_DOUBLEBUFFER, True,
	None
};

void static
xcb_window_creat(void *__ctx, ff_uint_t __width, ff_uint_t __height, char const *__title) {
	ffly_printf("window, width: %u, height: %u, title: %s\n", __width, __height, __title);
	struct ffly_xcb_ctx *ctx;
	ctx = (struct ffly_xcb_ctx*)__ctx;

	ctx->frame_buff = __ffly_mem_alloc(__width*__height*4);
	ctx->width = __width;
	ctx->height = __height;
	ctx->title = __title;
	xcb_setup_t *setup = xcb_get_setup(ctx->conn);

	xcb_screen_iterator_t screen_itr = xcb_setup_roots_iterator(setup);
	int def_screen = DefaultScreen(ctx->d);
	int screen_num = def_screen;
	for(;screen_itr.rem && screen_num>0;--screen_num)
		xcb_screen_next(&screen_itr);

	ctx->screen = screen_itr.data;
	ctx->w = xcb_generate_id(ctx->conn);

	int visual;
	GLXFBConfig *fb_configs;
	int fb_config_c;
	if (!(fb_configs = glXChooseFBConfig(ctx->d, def_screen, visatt, &fb_config_c))) {
		ffly_fprintf(ffly_err, "failed to get frame buffer config.\n");
		return;
	}

	if (!fb_config_c) return;

	GLXFBConfig fb_config = *fb_configs;
	glXGetFBConfigAttrib(ctx->d, fb_config, GLX_VISUAL_ID, &visual);
	if (!(ctx->glx_ct = glXCreateNewContext(ctx->d, fb_config, GLX_RGBA_TYPE, 0, True))) {
		ffly_fprintf(ffly_err, "failed to create glx context.\n");
		return;
	}

	xcb_colormap_t colour_map;
	colour_map = xcb_generate_id(ctx->conn);

	xcb_create_colormap(ctx->conn, 
					XCB_COLORMAP_ALLOC_NONE,
					colour_map,
					ctx->screen->root,
					visual);
	ff_u32_t event_mask = XCB_EVENT_MASK_EXPOSURE|XCB_EVENT_MASK_KEY_PRESS|XCB_EVENT_MASK_KEY_RELEASE|XCB_EVENT_MASK_BUTTON_PRESS|XCB_EVENT_MASK_BUTTON_RELEASE;
	ff_u32_t values[] = {event_mask, colour_map, XCB_EVENT_MASK_EXPOSURE, 0};
	ff_u32_t mask = XCB_CW_EVENT_MASK|XCB_CW_COLORMAP;

	xcb_create_window(ctx->conn,
					XCB_COPY_FROM_PARENT,
					ctx->w,
					ctx->screen->root,
					0,
					0,
					ctx->width,
					ctx->height,
					0,
					XCB_WINDOW_CLASS_INPUT_OUTPUT,
					visual,
					mask,
					values);

	xcb_change_property(ctx->conn,
					XCB_PROP_MODE_REPLACE,
					ctx->w,
					XCB_ATOM_WM_NAME,
					XCB_ATOM_STRING,
					8,
					ffly_str_len(ctx->title),
					ctx->title);
/* de comment later
	// size hints
	xcb_size_hints_t size_hints;
	xcb_icccm_size_hints_set_min_size(&size_hints, ctx->mn_width, ctx->mn_height);
	xcb_icccm_size_hints_set_max_size(&size_hints, ctx->mx_width, ctx->mx_height);
	xcb_icccm_set_wm_size_hints(ctx->conn, ctx->w, XCB_ATOM_WM_NORMAL_HINTS, &size_hints);
*/

	xcb_intern_atom_cookie_t proto_cookie = xcb_intern_atom(ctx->conn, 1, 12, "WM_PROTOCOLS");
	xcb_intern_atom_reply_t *proto_re = xcb_intern_atom_reply(ctx->conn, proto_cookie, 0);

	xcb_intern_atom_cookie_t cookie = xcb_intern_atom(ctx->conn, 0, 16, "WM_DELETE_WINDOW");
 	xcb_intern_atom_reply_t *re = xcb_intern_atom_reply(ctx->conn, cookie, 0);

	xcb_change_property(ctx->conn, XCB_PROP_MODE_REPLACE, ctx->w, proto_re->atom, 4, 32, 1, &re->atom);

	if (!(ctx->glx_window = glXCreateWindow(ctx->d, fb_config, ctx->w, NULL))) {
		return;
	}

	ctx->glx_dr = ctx->glx_window;
	if(!glXMakeContextCurrent(ctx->d, ctx->glx_dr, ctx->glx_dr, ctx->glx_ct)) {
		return;
	}
	xcb_map_window(ctx->conn, ctx->w);
	xcb_flush(ctx->conn);
	glViewport(0, 0, ctx->width, ctx->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, ctx->width, 0, ctx->height, 0.1, 1);
	glPixelZoom(1, -1);
	glRasterPos3f(0, ctx->height, -0.3);
}

void static
xcb_display_open(void *__ctx) {
	ffly_printf("display open.\n");
	struct ffly_xcb_ctx *ctx;
	ctx = (struct ffly_xcb_ctx*)__ctx;
	if (!(ctx->d = XOpenDisplay(NULL))) {
		ffly_fprintf(ffly_err, "failed to open display.\n");
		return;
	}

	if (!(ctx->conn = XGetXCBConnection(ctx->d))) {
		XCloseDisplay(ctx->d);
		ffly_fprintf(ffly_err, "failed to get connection.\n");
		return;
	}
	XSetEventQueueOwner(ctx->d, XCBOwnsEventQueue);
}

void static
xcb_display_close(void *__ctx) {
	ffly_printf("display close.\n");
	struct ffly_xcb_ctx *ctx;
	ctx = (struct ffly_xcb_ctx*)__ctx;
	XCloseDisplay(ctx->d);
}

void static
xcb_window_destroy(void *__ctx) {
	struct ffly_xcb_ctx *ctx;
	ctx = (struct ffly_xcb_ctx*)__ctx;
	glXDestroyContext(ctx->d, ctx->glx_ct);
	glXDestroyWindow(ctx->d, ctx->glx_window);
	xcb_destroy_window(ctx->conn, ctx->w);
	__ffly_mem_free(ctx->frame_buff);
}

void static
xcb_window_display(void *__ctx) {
	struct ffly_xcb_ctx *ctx;
	ctx = (struct ffly_xcb_ctx*)__ctx;

	glDrawPixels(ctx->width, ctx->height, GL_RGBA, GL_UNSIGNED_BYTE, ctx->frame_buff);
	glXSwapBuffers(ctx->d, ctx->glx_dr);
}

void static
xcb_cleanup(void *__ctx) {
	struct ffly_xcb_ctx *ctx;
	ctx = (struct ffly_xcb_ctx*)__ctx;
}

ff_u8_t static*
xcb_frame_buff(void *__ctx) {
	return ((struct ffly_xcb_ctx*)__ctx)->frame_buff;
}

static struct ffly_xcb_ctx xcb_ctx;
void ffly_mare_xcb(ffly_mctxp __m) {
	__m->driver.display_open = xcb_display_open;
	__m->driver.display_close = xcb_display_close;
	__m->driver.init = xcb_init;
	__m->driver.de_init = xcb_de_init;
	__m->driver.cleanup = xcb_cleanup;
	__m->driver.window_creat = xcb_window_creat;
	__m->driver.window_destroy = xcb_window_destroy;
	__m->driver.window_display = xcb_window_display;
	__m->driver.frame_buff = xcb_frame_buff;
	__m->context = (void*)&xcb_ctx;
}
