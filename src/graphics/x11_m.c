# include "x11_m.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../event.h"
# include "../system/io.h"
# include "../system/nanosleep.h"
# include "../dep/mem_set.h"
# include "../dep/str_len.h"
# include "../dep/mem_dup.h"
# include "mare.h"

int static
x11_err(Display *__d, XErrorEvent *__e) {
	printf("err code: %u\n", __e->error_code);
	switch(__e->error_code) {
		case BadAccess:
			ffly_fprintf(ffly_err, "bad access\n");
		break;
		case BadAlloc:
			ffly_fprintf(ffly_err, "bad alloc\n");
		break;
		case BadImplementation:
			ffly_fprintf(ffly_err, "bad implementation\n");
		break;
		case BadName:
			ffly_fprintf(ffly_err, "bad name\n");
		break;
		case BadValue:
			ffly_fprintf(ffly_err, "bad value\n");
		break;
		case BadLength:
			ffly_fprintf(ffly_err, "bad length\n");
		break;
		case BadMatch:
			ffly_fprintf(ffly_err, "bad match\n");
		break;
		case BadWindow:
			ffly_fprintf(ffly_err, "bad window\n");
		break;
	}
}

void static
x11_init(void *__ctx) {
	XSetErrorHandler(x11_err);
	ffly_mem_set(__ctx, 0, sizeof(struct ffly_x11_ctx));
}

void static
x11_de_init(void *__ctx) {

}

void static
x11_display_open(void *__ctx) {
	struct ffly_x11_ctx *ctx;
	ctx = (struct ffly_x11_ctx*)__ctx;
	if (!(ctx->d = XOpenDisplay(NULL))) {
		ffly_fprintf(ffly_err, "failed to open x11 display.\n");
		return;
	}
}

void static
x11_display_close(void *__ctx) {
	struct ffly_x11_ctx *ctx;
	ctx = (struct ffly_x11_ctx*)__ctx;
	XSetCloseDownMode(ctx->d, DestroyAll);
	XCloseDisplay(ctx->d);
}

void static
x11_window_creat(void *__ctx, ff_uint_t __width, ff_uint_t __height, char const *__title) {
	struct ffly_x11_ctx *ctx;
	ctx = (struct ffly_x11_ctx*)__ctx;
	ctx->frame_buff = (ff_u8_t*)__ffly_mem_alloc(__width*__height*4);

	ctx->width =  __width;
	ctx->height = __height;
	ctx->title = __title;
	XSetWindowAttributes wd_att;
	XVisualInfo *vis_info;

	GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
	if (!(vis_info = glXChooseVisual(ctx->d, 0, att))) {
		return;
	}

	Window root = XRootWindow(ctx->d, 0);
	wd_att.colormap = XCreateColormap(ctx->d, root, vis_info->visual, AllocNone);
	wd_att.event_mask = 0;

	ctx->w = XCreateWindow(ctx->d, root, 0, 0, ctx->width, ctx->height, 0, vis_info->depth, InputOutput, vis_info->visual, CWColormap|CWEventMask, &wd_att);
	XStoreName(ctx->d, ctx->w, ctx->title);
	XSelectInput(ctx->d, ctx->w, ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|StructureNotifyMask);
	XMapWindow(ctx->d, ctx->w);

	Atom WM_DELETE_WINDOW = XInternAtom(ctx->d, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(ctx->d, ctx->w, &WM_DELETE_WINDOW, 1);

/* de comment later
	XSizeHints size_hints = {
		.min_width = ctx->mn_width,
		.min_height = ctx->mn_height,
		.max_width = ctx->mx_width,
		.max_height = ctx->mx_height,
		.flags = PMinSize|PMaxSize
	};

	XSetWMNormalHints(ctx->d, ctx->w, &size_hints);
*/
	/*
		TODO:
			store gl context in struct as it need to be Destoryed
	*/
	GLXContext glx_ct = glXCreateContext(ctx->d, vis_info, NULL, GL_TRUE);
	if (!glx_ct) {
		return;
	}

	ctx->glx_ct = glx_ct;
	glXMakeCurrent(ctx->d, ctx->w, glx_ct);
	glViewport(0, 0, ctx->width, ctx->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, ctx->width, 0, ctx->height, 0.1, 1);
	glPixelZoom(1, -1);
	glRasterPos3f(0, ctx->height, -0.3);
}

void static
x11_window_destroy(void *__ctx) {
	struct ffly_x11_ctx *ctx;
	ctx = (struct ffly_x11_ctx*)__ctx;
	glXDestroyContext(ctx->d, ctx->glx_ct);
	XDestroyWindow(ctx->d, ctx->w);
	__ffly_mem_free(ctx->frame_buff);
}

void static
x11_cleanup(void *__ctx) {
	struct ffly_x11_ctx *ctx;
	ctx = (struct ffly_x11_ctx*)__ctx;
}

void static
x11_window_display(void *__ctx) {
    struct ffly_x11_ctx *ctx;
	ctx = (struct ffly_x11_ctx*)__ctx;
	glDrawPixels(ctx->width, ctx->height, GL_RGBA, GL_UNSIGNED_BYTE, ctx->frame_buff);
	glXSwapBuffers(ctx->d, ctx->w);
}

void static
x11_get(ff_u8_t __what, long long __arg, void *__ctx) {
	struct ffly_x11_ctx *ctx;
	ctx = (struct ffly_x11_ctx*)__ctx;

	switch(__what) {
		case 0x00:
			*(void**)__arg = ctx->frame_buff;
		break;
		case 0x01:
			*(ff_uint_t*)__arg = ctx->width;
		break;
		case 0x02:
			*(ff_uint_t*)__arg = ctx->height;
		break;
	}
}

static struct ffly_x11_ctx x11_ctx;
void ffly_mare_x11(ffly_mctxp __m) {
	__m->driver.display_open = x11_display_open;
	__m->driver.display_close = x11_display_close;
	__m->driver.init = x11_init;
	__m->driver.de_init = x11_de_init;
	__m->driver.cleanup = x11_cleanup;
	__m->driver.window_creat = x11_window_creat;
	__m->driver.window_destroy = x11_window_destroy;
	__m->driver.window_display = x11_window_display;
	__m->driver.get = x11_get;
	__m->context = (void*)&x11_ctx;
}
