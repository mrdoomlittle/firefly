# include "x11_wd.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "wd_flags.h"
# include "../types/event_t.h"
# include "../system/io.h"
# include "../system/event_kind.h"
# define EVENT_BUFF_SIZE 20

int static ffly_x11_err_handle(Display *__d, XErrorEvent *__e) {
	printf("err code: %u\n", __e->error_code);
	switch(__e->error_code) {
		case BadAccess:
			printf("bad access\n");
		break;
		case BadAlloc:
			printf("bad alloc\n");
		break;
		case BadImplementation:
			printf("bad implementation\n");
		break;
		case BadName:
			printf("bad name\n");
		break;
		case BadValue:
			printf("bad value\n");
		break;
		case BadLength:
			printf("bad length\n");
		break;
		case BadMatch:
			printf("bad match\n");
		break;
		case BadWindow:
			printf("bad window\n");
		break;
	}
}

ffly_err_t ffly_x11_wd_begin(struct ffly_x11_wd *__x11_wd, mdl_u16_t __xa_len, mdl_u16_t __ya_len, char const *__title) {
	XSetErrorHandler(ffly_x11_err_handle);
	__x11_wd->xa_len = __xa_len;
	__x11_wd->ya_len = __ya_len;

	if (ffly_buff_init(&__x11_wd->event_buff, EVENT_BUFF_SIZE, sizeof(ffly_event_t)) != FFLY_SUCCESS) return FFLY_FAILURE;

	if (!(__x11_wd->d = XOpenDisplay(NULL))) {
		return FFLY_FAILURE;
	}

	XSetWindowAttributes w_att;
	XVisualInfo *vis_info;

	GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

	if (!(vis_info = glXChooseVisual(__x11_wd->d, 0, att))) {
		return FFLY_FAILURE;
	}

	Window rt = XRootWindow(__x11_wd->d, 0);
	w_att.colormap = XCreateColormap(__x11_wd->d, rt, vis_info->visual, AllocNone);
	w_att.event_mask = 0x0;

	__x11_wd->w = XCreateWindow(__x11_wd->d, rt, 0, 0, __xa_len, __ya_len, 0, vis_info->depth, InputOutput, vis_info->visual, CWColormap|CWEventMask, &w_att);
	XStoreName(__x11_wd->d, __x11_wd->w, __title);
	XSelectInput(__x11_wd->d, __x11_wd->w, ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|StructureNotifyMask);
	XMapWindow(__x11_wd->d, __x11_wd->w);

	Atom WM_DELETE_WINDOW = XInternAtom(__x11_wd->d, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(__x11_wd->d, __x11_wd->w, &WM_DELETE_WINDOW, 1);

	XSizeHints size_hints = {
		.min_width = __xa_len-__x11_wd->mn_xal_off,
		.min_height = __ya_len-__x11_wd->mn_yal_off,
		.max_width = __xa_len+__x11_wd->mx_xal_off,
		.max_height = __ya_len+__x11_wd->mx_yal_off,
		.flags = PMinSize|PMaxSize
	};

	XSetWMNormalHints(__x11_wd->d, __x11_wd->w, &size_hints);

	GLXContext glx_ct = glXCreateContext(__x11_wd->d, vis_info, NULL, GL_TRUE);
	if (!glx_ct) {
		return FFLY_FAILURE;
	}

	glXMakeCurrent(__x11_wd->d, __x11_wd->w, glx_ct);
	glViewport(0, 0, __xa_len, __ya_len);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, __xa_len, 0, __ya_len, 0.1, 1);
	glPixelZoom(1, -1);
	glRasterPos3f(0, __ya_len, -0.3);

	if ((__x11_wd->frame_buff = __ffly_mem_alloc(__xa_len*__ya_len*4)) == NULL) {
		XDestroyWindow(__x11_wd->d, __x11_wd->w);
		XCloseDisplay(__x11_wd->d);
		return FFLY_FAILURE;
	}

	ffly_add_flag(&__x11_wd->flags, FLG_WD_OPEN, 1);

	XEvent xev;
	ffly_event_t event;
	do {
		while (XPending(__x11_wd->d) > 0) {
			XNextEvent(__x11_wd->d, &xev);
			switch(xev.type) {
				case ClientMessage:
					goto _end;
				case KeyPress:
					event = (ffly_event_t) {.kind = _ffly_ek_key_press};
				break;
				case KeyRelease:
					event = (ffly_event_t) {.kind = _ffly_ek_key_release};
				break;
				default:
					continue;
			}

			if (ffly_buff_put(&__x11_wd->event_buff, (void*)&event) != FFLY_SUCCESS) {
				ffly_printf(stderr, "x11_wd, failed to put event into buffer.\n");
				continue;
			}

			if (ffly_buff_incr(&__x11_wd->event_buff) != FFLY_SUCCESS) {
				ffly_printf(stderr, "x11_wd, failed to increment buff to next block.\n");
			}
		}

		ffly_add_flag(&__x11_wd->flags, FLG_WD_WAITING, 0);
		if (!ffly_is_flag(__x11_wd->flags, FLG_WD_DONE_DRAW)) continue;

		glDrawPixels(__xa_len, __ya_len, GL_RGBA, GL_UNSIGNED_BYTE, __x11_wd->frame_buff);
		glXSwapBuffers(__x11_wd->d, __x11_wd->w);

		ffly_rm_flag(&__x11_wd->flags, FLG_WD_DONE_DRAW);
		if (ffly_is_flag(__x11_wd->flags, FLG_WD_WAITING)) continue;

	} while(!ffly_is_flag(__x11_wd->flags, FLG_WD_TO_CLOSE));

	_end:
	ffly_add_flag(&__x11_wd->flags, FLG_WD_CLOSED, 0);
	XDestroyWindow(__x11_wd->d, __x11_wd->w);
	XCloseDisplay(__x11_wd->d);
	__ffly_mem_free(__x11_wd->frame_buff);
}
