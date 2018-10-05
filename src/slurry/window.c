# include "slurry.h"
# include "window.h"
# include <stdio.h>
# include <malloc.h>
struct s_window* __s_window_new(Display *__d) {
	struct s_window *wd;
	wd = (struct s_window*)malloc(sizeof(struct s_window));
	wd->next = NULL;
	wd->d = __d;
	return wd;
}

void __s_window_init(struct s_window *__wd, ff_uint_t __width, ff_uint_t __height, char const *__title) {
	__wd->frame_buff = (ff_u8_t*)malloc(__width*__height*4);
	__wd->width = __width;
	__wd->height = __height;
	XSetWindowAttributes wd_att;
	XVisualInfo *vis_info;

	GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
	if (!(vis_info = glXChooseVisual(__wd->d, 0, att))) {
		return;
	}

	Window root = XRootWindow(__wd->d, 0);
	wd_att.colormap = XCreateColormap(__wd->d, root, vis_info->visual, AllocNone);
	wd_att.event_mask = 0;

	__wd->w = XCreateWindow(__wd->d, root, 0, 0, __wd->width, __wd->height, 0, vis_info->depth, InputOutput, vis_info->visual, CWColormap|CWEventMask, &wd_att);
	XStoreName(__wd->d, __wd->w, __title);
	XSelectInput(__wd->d, __wd->w, ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|StructureNotifyMask);
	XMapWindow(__wd->d, __wd->w);

	Atom WM_DELETE_WINDOW = XInternAtom(__wd->d, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(__wd->d, __wd->w, &WM_DELETE_WINDOW, 1);

	GLXContext glx_ct = glXCreateContext(__wd->d, vis_info, NULL, GL_TRUE);
	if (!glx_ct) {
		return;
	}

	__wd->glx_ct = glx_ct;
	glXMakeCurrent(__wd->d, __wd->w, glx_ct);
	glViewport(0, 0, __wd->width, __wd->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, __wd->width, 0, __wd->height, 0.1, 1);
	glPixelZoom(1, -1);
	glRasterPos3f(0, __wd->height, -0.3);
}

void __s_window_display(struct s_window *__wd) {
	glDrawPixels(__wd->width, __wd->height, GL_RGBA, GL_UNSIGNED_BYTE, __wd->frame_buff);
	glXSwapBuffers(__wd->d, __wd->w);
}

void __s_window_destroy(struct s_window *__wd) {
	glXDestroyContext(__wd->d, __wd->glx_ct);
	XDestroyWindow(__wd->d, __wd->w);
	free(__wd->frame_buff);
	free(__wd);
}
