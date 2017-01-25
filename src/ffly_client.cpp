# include <X11/Xlib.h>
# include <GL/gl.h>
# include <GL/glx.h>
# include <GL/glu.h>
# include <cstdio>
# include <boost/cstdint.hpp>

# define WX_LEN 200
# define WY_LEN 200

int main() {
	XVisualInfo * _vis_info;

	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

	XSetWindowAttributes _win_att;

	Colormap _color_map;

	Display * _display;
	Window _window;
	XEvent _xevent;
	Window _rwindow;

	_display = XOpenDisplay(NULL);
	if (_display == NULL) {
		printf("display error.\n");
		return 1;
	}

	_rwindow = XRootWindow(_display, 0);

	int _screen = DefaultScreen(_display);

	_vis_info = glXChooseVisual(_display, 0, att);

	_color_map = XCreateColormap(_display, _rwindow, _vis_info-> visual, AllocNone);

	_win_att.colormap = _color_map;
	_win_att.event_mask = ExposureMask | KeyPressMask;

	_window = XCreateWindow(_display, _rwindow, 0, 0, WX_LEN, WY_LEN, 0, _vis_info-> depth, InputOutput, _vis_info-> visual, CWColormap | CWEventMask, &_win_att);

	XMapWindow(_display, _window);

	Atom WM_DELETE_WINDOW = XInternAtom(_display, "WM_DELETE_WINDOW", false);

	XSetWMProtocols(_display, _window, &WM_DELETE_WINDOW, 1);

	XStoreName(_display, _window, "FireFly Client");

	XSizeHints _size_hints;

	_size_hints.width = _size_hints.min_width = _size_hints.max_width = WX_LEN;
	_size_hints.height = _size_hints.min_height = _size_hints.max_height = WY_LEN;

	_size_hints.flags = PSize | PMinSize | PMaxSize | PPosition;

	XSetWMNormalHints(_display, _window, &_size_hints);

	GLXContext _glx_context = glXCreateContext(_display, _vis_info, NULL, GL_TRUE);
	glXMakeCurrent(_display, _window, _glx_context);

	for (;;) {
		while (XPending(_display) > 0) {
			XNextEvent(_display, &_xevent);
			if (_xevent.type == KeyPress || _xevent.type == ClientMessage) goto end;
		}

		// slow things down
		for (int x = 0; x != 10000000; x ++) {}


		printf("tick.\n");
		glXSwapBuffers(_display, _window);
	}
	end:

	XDestroyWindow(_display, _window);
	XCloseDisplay(_display);

	return 0;
}
