# include "x11_wd.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../types/event_t.h"
# include "../system/io.h"
# include "../system/event_kind.h"
# include "../system/event_field.h"
# include "../system/nanosleep.h"
# include "../dep/mem_set.h"
# include "../dep/str_len.h"
# include "../dep/mem_dup.h"
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

ff_err_t
ffly_x11_wd_init(struct ffly_x11_wd *__wd, ff_u16_t __width,
	ff_u16_t __height, char const *__title)
{
	XSetErrorHandler(x11_err);
	ffly_mem_set(__wd, 0x0, sizeof(struct ffly_x11_wd));
	__wd->width = __width;
	__wd->height = __height;
	__wd->title = __title;
	if ((__wd->frame_buff = __ffly_mem_alloc(__width*__height*4)) == NULL) {
		ffly_fprintf(ffly_err, "failed to allocate memory for frame buffer.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ff_err_t ffly_x11_wd_open(struct ffly_x11_wd *__wd) {
	if (!(__wd->d = XOpenDisplay(NULL))) {
		ffly_fprintf(ffly_err, "failed to open display.\n");
		return FFLY_FAILURE;
	}

	XSetWindowAttributes wd_att;
	XVisualInfo *vis_info;

	GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
	if (!(vis_info = glXChooseVisual(__wd->d, 0, att))) {
		return FFLY_FAILURE;
	}

	Window root = XRootWindow(__wd->d, 0);
	wd_att.colormap = XCreateColormap(__wd->d, root, vis_info->visual, AllocNone);
	wd_att.event_mask = 0;

	__wd->w = XCreateWindow(__wd->d, root, 0, 0, __wd->width, __wd->height, 0, vis_info->depth, InputOutput, vis_info->visual, CWColormap|CWEventMask, &wd_att);
	XStoreName(__wd->d, __wd->w, __wd->title);
	XSelectInput(__wd->d, __wd->w, ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|StructureNotifyMask);
	XMapWindow(__wd->d, __wd->w);

	Atom WM_DELETE_WINDOW = XInternAtom(__wd->d, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(__wd->d, __wd->w, &WM_DELETE_WINDOW, 1);

/* de comment later
	XSizeHints size_hints = {
		.min_width = __wd->mn_width,
		.min_height = __wd->mn_height,
		.max_width = __wd->mx_width,
		.max_height = __wd->mx_height,
		.flags = PMinSize|PMaxSize
	};

	XSetWMNormalHints(__wd->d, __wd->w, &size_hints);
*/
	GLXContext glx_ct = glXCreateContext(__wd->d, vis_info, NULL, GL_TRUE);
	if (!glx_ct) {
		return FFLY_FAILURE;
	}

	glXMakeCurrent(__wd->d, __wd->w, glx_ct);
	glViewport(0, 0, __wd->width, __wd->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, __wd->width, 0, __wd->height, 0.1, 1);
	glPixelZoom(1, -1);
	glRasterPos3f(0, __wd->height, -0.3);
	return FFLY_SUCCESS;
}

ff_err_t ffly_x11_wd_close(struct ffly_x11_wd *__wd) {
	XSetCloseDownMode(__wd->d, DestroyAll);
	XDestroyWindow(__wd->d, __wd->w);
	XCloseDisplay(__wd->d);
	return FFLY_SUCCESS;
}

ff_err_t ffly_x11_wd_cleanup(struct ffly_x11_wd *__wd) {
	__ffly_mem_free(__wd->frame_buff);
	return FFLY_SUCCESS;
}

