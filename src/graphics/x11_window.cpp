# include "x11_window.hpp"

// NOTE: clean up
boost::int8_t mdl::firefly::graphics::x11_window::init(uint_t __wd_xlen, uint_t __wd_ylen, char const *__frame_title) {
//	if (this-> init_called) return 1;

	this-> add_wd_flag(X11_WD_OPEN, true);

	this-> wd_xlen = __wd_xlen;
	this-> wd_ylen = __wd_ylen;
	this-> frame_title = __frame_title;

	this-> pixbuff = memory::alloc_pixmap(__wd_xlen, __wd_ylen, 1);
	//this-> pixbuff = static_cast<boost::uint8_t *>(malloc((__wd_xlen * __wd_ylen) * 4));
	bzero(this-> pixbuff, (__wd_xlen * __wd_ylen) * 4);

	XVisualInfo * _vis_info;

	printf("creating window with size %dx%d and a title of '%s'\n", __wd_ylen, __wd_xlen, __frame_title);

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
        return -1;
    }

	int _screen = DefaultScreen(_display);

    _rwindow = XRootWindow(_display, 0);

    _vis_info = glXChooseVisual(_display, 0, att);

    _color_map = XCreateColormap(_display, _rwindow, _vis_info-> visual, AllocNone);

    _win_att.colormap = _color_map;
    _win_att.event_mask = ExposureMask | KeyPressMask;

    _window = XCreateWindow(_display, _rwindow, 0, 0, __wd_xlen, __wd_ylen, 0, _vis_info-> depth, InputOutput, _vis_info-> visual, CWColormap | CWEventMask, &_win_att);

	XSelectInput(_display, _window, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);

    XMapWindow(_display, _window);

    Atom WM_DELETE_WINDOW = XInternAtom(_display, "WM_DELETE_WINDOW", false);

    XSetWMProtocols(_display, _window, &WM_DELETE_WINDOW, 1);

    XStoreName(_display, _window, __frame_title);

    XSizeHints _size_hints;

    _size_hints.width = _size_hints.min_width = _size_hints.max_width = __wd_xlen;
    _size_hints.height = _size_hints.min_height = _size_hints.max_height = __wd_ylen;

    _size_hints.flags = PSize | PMinSize | PMaxSize | PPosition;

    XSetWMNormalHints(_display, _window, &_size_hints);

    GLXContext _glx_context = glXCreateContext(_display, _vis_info, NULL, GL_TRUE);
    glXMakeCurrent(_display, _window, _glx_context);

	int unsigned screen_count = XScreenCount(_display);

    Window * r_windows = static_cast<Window *>(malloc(screen_count * sizeof(Window)));

    for (std::size_t i = 0; i != screen_count; i ++) {
        r_windows[i] = XRootWindow(_display, i);
    }

	glViewport(0, 0, __wd_xlen, __wd_ylen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, __wd_xlen, 0, __wd_ylen, 0.1, 1);
	glPixelZoom(1, -1);
	glRasterPos3f(0, __wd_ylen, -0.3);

	int w_xpos, w_ypos, m_xpos, m_ypos, r_mxpos, r_mypos;
	int unsigned w_width, w_height, w_border_width, w_depth, m_mask;
	Window child, root = XRootWindow(_display, 0);

	auto begin = std::chrono::high_resolution_clock::now();

	uint_t nanoseconds_in_sec = 100000000;
	double time_each_frame = (double(nanoseconds_in_sec)/double(this-> fps_mark));
	while(!this-> is_wd_flag(X11_WD_TO_CLOSE))
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
		if (duration.count() < time_each_frame) continue;

		this-> add_wd_flag(X11_WD_WAITING);
		this-> rm_wd_flag(X11_WD_DONE_DRAW);

		XTranslateCoordinates(_display, _window, root, 0, 0, &this-> window_coords.xaxis, &this-> window_coords.yaxis, &child);

		for (std::size_t o = 0; o != screen_count; o ++) {
			bool resault = XQueryPointer(_display, r_windows[o], &_rwindow, &child, &this-> mouse_coords.r_xaxis, &this-> mouse_coords.r_yaxis, &this-> mouse_coords.s_xaxis, &this-> mouse_coords.s_yaxis, &m_mask);
			if (resault) break;
		}

		if ((this-> mouse_coords.s_xaxis > this-> window_coords.xaxis && this-> mouse_coords.s_xaxis < (this-> window_coords.xaxis + __wd_xlen))
		&& (this-> mouse_coords.s_yaxis > this-> window_coords.yaxis && this-> mouse_coords.s_yaxis < (this-> window_coords.yaxis + __wd_ylen))) {

			this-> mouse_coords.w_xaxis = (this-> mouse_coords.s_xaxis - this-> window_coords.xaxis);
			this-> mouse_coords.w_yaxis = (this-> mouse_coords.s_yaxis - this-> window_coords.yaxis);
		} else {
			this-> mouse_coords.w_xaxis = 0;
            this-> mouse_coords.w_yaxis = 0;
		}

		while (XPending(_display) > 0) {
            XNextEvent(_display, &_xevent);
            if (_xevent.type == ClientMessage) goto end;
			if (_xevent.type == KeyPress) {
				this-> key_press = true;
				this-> key_code = _xevent.xkey.keycode;
			} else if (_xevent.type == KeyRelease) {
				this-> key_press = false;
				this-> key_code = 0x0;
			} else if (_xevent.type == ButtonPress) {
				this-> button_press = true;
				this-> button_code = _xevent.xbutton.button;
			} else if (_xevent.type == ButtonRelease) {
				this-> button_press = false;
				this-> button_code = 0x0;
			}

			if (this-> key_press) printf("key press: %d\n", this-> key_code);
        }

		if (!this-> is_wd_flag(X11_WD_DONE_DRAW)) continue;

		glDrawPixels(__wd_xlen, __wd_ylen, GL_RGBA, GL_UNSIGNED_BYTE, this-> pixbuff);

		glXSwapBuffers(_display, _window);

		begin = std::chrono::high_resolution_clock::now();

		if (this-> is_wd_flag(X11_WD_WAITING)) continue;
	}

	end:

	printf("closing window.\n");

	glXMakeCurrent(_display, None, NULL);
    glXDestroyContext(_display, _glx_context);
    XDestroyWindow(_display, _window);
    XCloseDisplay(_display);
    std::free(r_windows);

	memory::mem_free(this-> pixbuff);

	this-> add_wd_flag(X11_WD_CLOSED, true);
}

boost::int8_t mdl::firefly::graphics::x11_window::begin(uint_t __wd_xlen, uint_t __wd_ylen, char const *__frame_title) {
	boost::thread(boost::bind(&x11_window::init, this, __wd_xlen, __wd_ylen, __frame_title));
}
