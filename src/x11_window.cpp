# include "x11_window.hpp"
boost::uint8_t mdl::x11_window::init(boost::uint16_t __wx_axis_len, boost::uint16_t __wy_axis_len, char const *__title) {
	if (this-> init_called) return 1;

	this-> pixels = static_cast<boost::uint8_t *>(malloc((__wx_axis_len * __wy_axis_len) * 4));
	memset(this-> pixels, 0x0, (__wx_axis_len * __wy_axis_len) * 4);

	XVisualInfo * _vis_info;

	GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

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

	int _screen = DefaultScreen(_display);

    _rwindow = XRootWindow(_display, 0);

    _vis_info = glXChooseVisual(_display, 0, att);

    _color_map = XCreateColormap(_display, _rwindow, _vis_info-> visual, AllocNone);

    _win_att.colormap = _color_map;
    _win_att.event_mask = ExposureMask | KeyPressMask;

    _window = XCreateWindow(_display, _rwindow, 0, 0, __wx_axis_len, __wy_axis_len, 0, _vis_info-> depth, InputOutput, _vis_info-> visual, CWColormap | CWEventMask, &_win_att);

	XSelectInput(_display, _window, KeyPressMask | KeyReleaseMask);

    XMapWindow(_display, _window);

    Atom WM_DELETE_WINDOW = XInternAtom(_display, "WM_DELETE_WINDOW", false);

    XSetWMProtocols(_display, _window, &WM_DELETE_WINDOW, 1);

    XStoreName(_display, _window, __title);

    XSizeHints _size_hints;

    _size_hints.width = _size_hints.min_width = _size_hints.max_width = __wx_axis_len;
    _size_hints.height = _size_hints.min_height = _size_hints.max_height = __wy_axis_len;

    _size_hints.flags = PSize | PMinSize | PMaxSize | PPosition;

    XSetWMNormalHints(_display, _window, &_size_hints);

    GLXContext _glx_context = glXCreateContext(_display, _vis_info, NULL, GL_TRUE);
    glXMakeCurrent(_display, _window, _glx_context);

	int unsigned screen_count = XScreenCount(_display);

    Window * r_windows = static_cast<Window *>(malloc(screen_count * sizeof(Window)));

    for (std::size_t i = 0; i != screen_count; i ++) {
        r_windows[i] = XRootWindow(_display, i);
    }

	int w_xpos, w_ypos, m_xpos, m_ypos, r_mxpos, r_mypos;
	int unsigned w_width, w_height, w_border_width, w_depth, m_mask;
	Window child, root = XRootWindow(_display, 0);
	while(true)
	{
		XTranslateCoordinates(_display, _window, root, 0, 0, &this-> window_coords.xaxis, &this-> window_coords.yaxis, &child);

		for (std::size_t o = 0; o != screen_count; o ++) {
			bool resault = XQueryPointer(_display, r_windows[o], &_rwindow, &child, &this-> mouse_coords.r_xaxis, &this-> mouse_coords.r_yaxis, &this-> mouse_coords.s_xaxis, &this-> mouse_coords.s_yaxis, &m_mask);
			if (resault) break;
		}

		if ((this-> mouse_coords.s_xaxis > this-> window_coords.xaxis && this-> mouse_coords.s_xaxis < (this-> window_coords.xaxis + __wx_axis_len))
		&& (this-> mouse_coords.s_yaxis > this-> window_coords.yaxis && this-> mouse_coords.s_yaxis < (this-> window_coords.yaxis + __wy_axis_len))) {

			this-> mouse_coords.w_xaxis = (this-> mouse_coords.s_xaxis - this-> window_coords.xaxis);
			this-> mouse_coords.w_yaxis = (this-> mouse_coords.s_yaxis - this-> window_coords.yaxis);
		} else {
			this-> mouse_coords.w_xaxis = 0;
            this-> mouse_coords.w_yaxis = 0;
		}

		glDrawPixels(__wx_axis_len, __wy_axis_len, GL_RGBA, GL_BYTE, this-> pixels);

		while (XPending(_display) > 0) {
            XNextEvent(_display, &_xevent);
            if (_xevent.type == ClientMessage) goto end;
			if (_xevent.type == KeyPress) {
				this-> key_press = true;
				this-> key_code = _xevent.xkey.keycode;
			} else if (_xevent.type == KeyRelease) {
				this-> key_press = false;
				this-> key_code = 0x0;
			}

			if (this-> key_press) printf("key press: %d\n", this-> key_code);
        }

		glXSwapBuffers(_display, _window);
	}

	end:

	glXMakeCurrent(_display, None, NULL);
    glXDestroyContext(_display, _glx_context);
    XDestroyWindow(_display, _window);
    XCloseDisplay(_display);
    std::free(r_windows);

	pthread_cancel(this-> window_thread-> native_handle());

	this-> init_called = true;
}

boost::uint8_t mdl::x11_window::begin(boost::uint16_t __wx_axis_len, boost::uint16_t __wy_axis_len, char const *__title) {
	this-> window_thread = new boost::thread(boost::bind(&x11_window::init, this, __wx_axis_len, __wy_axis_len, __title));
}
