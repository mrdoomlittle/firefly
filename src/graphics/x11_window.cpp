# include "x11_window.hpp"
mdl::firefly::types::err_t mdl::firefly::graphics::x11_window::begin(u16_t __wd_xaxis_len, u16_t __wd_yaxis_len, char const *__frame_title) {
	printf("thread started.\n");
	if (this-> is_wd_flag(FLG_WD_KILLED)) return FFLY_SUCCESS;

	this-> wd_xaxis_len = __wd_xaxis_len;
	this-> wd_yaxis_len = __wd_yaxis_len;
	this-> frame_title = (char *)memory::mem_alloc(strlen(__frame_title));
	if (this-> frame_title == NULL) {
		fprintf(stderr, "x11_window: failed to alloc memory for replacment frame title, errno: %d\n", errno);
		this-> init_report = FFLY_FAILURE;
		return FFLY_FAILURE;
	}

	memcpy(this-> frame_title, __frame_title, strlen(__frame_title));

	this-> pixbuff = memory::alloc_pixmap((uint_t)__wd_xaxis_len, (uint_t)__wd_yaxis_len);
	if (this-> pixbuff == NULL) {
		fprintf(stderr, "x11_window: failed to alloc memory for pixbuff, errno: %d\n", errno);
		this-> init_report = FFLY_FAILURE;
		return FFLY_FAILURE;
	}

	bzero(this-> pixbuff, (__wd_xaxis_len * __wd_yaxis_len) * 4);

	XVisualInfo * _vis_info;

	printf("creating window with size %dx%d and a title of '%s'\n", __wd_yaxis_len, __wd_xaxis_len, __frame_title);

	GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

	XSetWindowAttributes _win_att;
	Colormap _color_map;

	Display * _display;
    Window _window;
    XEvent _xevent;
    Window _rwindow;

    if ((_display = XOpenDisplay(NULL)) == NULL) {
		this-> init_report = FFLY_FAILURE;
        printf("display error.\n");
        return FFLY_FAILURE;
    }
	this-> display = _display;

	int _screen = DefaultScreen(_display);

    _rwindow = XRootWindow(_display, 0);

	if ((_vis_info = glXChooseVisual(_display, 0, att)) == NULL) {
		this-> init_report = FFLY_FAILURE;
		fprintf(stderr, "failed to set visual info.\n");
		return FFLY_FAILURE;
	}

    _color_map = XCreateColormap(_display, _rwindow, _vis_info-> visual, AllocNone);

    _win_att.colormap = _color_map;
    _win_att.event_mask = 0x0;

    _window = XCreateWindow(_display, _rwindow, 0, 0, __wd_xaxis_len, __wd_yaxis_len, 0, _vis_info-> depth, InputOutput, _vis_info-> visual, CWColormap | CWEventMask, &_win_att);

	XSelectInput(_display, _window, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask);

    XMapWindow(_display, _window);

    Atom WM_DELETE_WINDOW = XInternAtom(_display, "WM_DELETE_WINDOW", false);

    XSetWMProtocols(_display, _window, &WM_DELETE_WINDOW, 1);

    XStoreName(_display, _window, __frame_title);

    XSizeHints _size_hints;

    _size_hints.width = _size_hints.min_width = _size_hints.max_width = __wd_xaxis_len;
    _size_hints.height = _size_hints.min_height = _size_hints.max_height = __wd_yaxis_len;

    _size_hints.flags = PSize | PMinSize | PMaxSize | PPosition;

    XSetWMNormalHints(_display, _window, &_size_hints);

    GLXContext _glx_context = glXCreateContext(_display, _vis_info, NULL, GL_TRUE);
	if (!_glx_context) {
		this-> init_report = FFLY_FAILURE;
		fprintf(stderr, "x11_window: failed to create glx context.\n");
		return FFLY_FAILURE;
	}

    glXMakeCurrent(_display, _window, _glx_context);

	int unsigned screen_count = XScreenCount(_display);

    Window *r_windows = static_cast<Window *>(malloc(screen_count * sizeof(Window)));
	if (r_windows == NULL) {
		this-> init_report = FFLY_FAILURE;
		fprintf(stderr, "x11_window: failed to alloc memory for 'r_windows', errno: %d\n", errno);
		return FFLY_FAILURE;
	}

    for (std::size_t i = 0; i != screen_count; i ++) {
        r_windows[i] = XRootWindow(_display, i);
    }

	glViewport(0, 0, __wd_xaxis_len, __wd_yaxis_len);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, __wd_xaxis_len, 0, __wd_yaxis_len, 0.1, 1);
	glPixelZoom(1, -1);
	glRasterPos3f(0, __wd_yaxis_len, -0.3);

	int w_xpos, w_ypos, m_xpos, m_ypos, r_mxpos, r_mypos;
	int unsigned w_width, w_height, w_border_width, w_depth, m_mask;

	system::stop_watch fps_timer;
	fps_timer.begin();

	uint_t time_each_frame = 100000000/this-> fps_mark;

	Window dmmy_child;
	this-> add_wd_flag(FLG_WD_OPEN, true);

	if (!this-> ev_queue)
		fprintf(stdout, "x11_window: warning event queue is empty.\n");

	do {
		XTranslateCoordinates(_display, _window, XRootWindow(_display, 0), 0, 0, &this-> wd_coords.xaxis, &this-> wd_coords.yaxis, &dmmy_child);

		for (std::size_t o = 0; o != screen_count; o ++) {
			Window dmmy_root;
			int dmmy_xaxis, dmmy_yaxis;
			if (XQueryPointer(_display, r_windows[o], &dmmy_root, &dmmy_child, &this-> mouse_coords.root.xaxis, &this-> mouse_coords.root.yaxis, &dmmy_xaxis, &dmmy_yaxis, &m_mask)) break;
		}

		if ((this-> mouse_coords.root.xaxis > this-> wd_coords.xaxis && this-> mouse_coords.root.xaxis < (this-> wd_coords.xaxis + __wd_xaxis_len))
			&& (this-> mouse_coords.root.yaxis > this-> wd_coords.yaxis && this-> mouse_coords.root.yaxis < (this-> wd_coords.yaxis + __wd_yaxis_len))) {
			this-> mouse_coords.wd.xaxis = (this-> mouse_coords.root.xaxis - this-> wd_coords.xaxis);
			this-> mouse_coords.wd.yaxis = (this-> mouse_coords.root.yaxis - this-> wd_coords.yaxis);

			this-> contains_pointer = true;
		} else
			this-> contains_pointer = false;

		while (XPending(_display) > 0) {
			if (this-> ev_queue != nullptr)
				if (this-> ev_queue-> size() > system::config.mx_queue_size) break;

			XNextEvent(_display, &_xevent);
			data::pair<types::event_desc_t, uint_t> event;
			bool unreg_event = false;
			switch(_xevent.type) {
				case ClientMessage:
					goto end;

				case KeyPress:
					event.first.event_id = system::KEY_EID;
					this-> key_press = true;
					this-> key_code = _xevent.xkey.keycode;
					event.second = _xevent.xkey.keycode;
					event.first.event_type = system::KEY_PRESS;
				break;

				case KeyRelease:
					event.first.event_id = system::KEY_EID;
					this-> key_press = false;
					event.second = _xevent.xkey.keycode;
					event.first.event_type = system::KEY_RELEASE;
				break;

				case ButtonPress:
					event.first.event_id = system::BTN_EID;
					this-> button_press = true;
					this-> button_code = _xevent.xbutton.button;
					event.second = _xevent.xbutton.button;
					event.first.event_type = system::BTN_PRESS;
				break;

				case ButtonRelease:
					event.first.event_id = system::BTN_EID;
					this-> button_press = false;
					event.second = _xevent.xbutton.button;
					event.first.event_type = system::BTN_RELEASE;
				break;
				default:
					unreg_event = true;
				break;
			}

			if (this-> ev_queue != nullptr && !unreg_event)
				this-> ev_queue-> push(event);
		}

		fps_timer.time_point();
		if (fps_timer.get_delta<std::chrono::nanoseconds>() < time_each_frame) continue;

		this-> add_wd_flag(FLG_WD_WAITING);

		if (!this-> is_wd_flag(FLG_WD_DONE_DRAW)) continue;

		glDrawPixels(__wd_xaxis_len, __wd_yaxis_len, GL_RGBA, GL_UNSIGNED_BYTE, this-> pixbuff);
		glXSwapBuffers(_display, _window);

		this-> rm_wd_flag(FLG_WD_DONE_DRAW);

		if (this-> is_wd_flag(FLG_WD_WAITING)) continue;

		fps_timer.begin();
	} while (!this-> is_wd_flag(FLG_WD_TO_CLOSE));
/*
	while(!this-> is_wd_flag(X11_WD_TO_CLOSE))
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
		if (duration.count() < time_each_frame) continue;

//		this-> add_wd_flag(X11_WD_WAITING);
//		this-> rm_wd_flag(X11_WD_DONE_DRAW);

		XTranslateCoordinates(_display, _window, XRootWindow(_display, 0), 0, 0, &this-> wd_coords.xaxis, &this-> wd_coords.yaxis, &dmmy_child);

		for (std::size_t o = 0; o != screen_count; o ++) {
			// X11 does not allow a null or nullptr to be passed thru without error
			Window dmmy_root;
			int dmmy_xaxis, dmmy_yaxis;
			if (XQueryPointer(_display, r_windows[o], &dmmy_root, &dmmy_child, &this-> mouse_coords.root.xaxis, &this-> mouse_coords.root.yaxis, &dmmy_xaxis, &dmmy_yaxis, &m_mask)) break;
		}

		if ((this-> mouse_coords.root.xaxis > this-> wd_coords.xaxis && this-> mouse_coords.root.xaxis < (this-> wd_coords.xaxis + __wd_xaxis_len))
			&& (this-> mouse_coords.root.yaxis > this-> wd_coords.yaxis && this-> mouse_coords.root.yaxis < (this-> wd_coords.yaxis + __wd_yaxis_len))) {

			this-> mouse_coords.wd.xaxis = (this-> mouse_coords.root.xaxis - this-> wd_coords.xaxis);
			this-> mouse_coords.wd.yaxis = (this-> mouse_coords.root.yaxis - this-> wd_coords.yaxis);

			this-> mouse_inside_wd = true;
		} else
			this-> mouse_inside_wd = false;

//		printf("mc_rx: %d, mc_ry: %d\n", this-> mouse_coords.root.xaxis, this-> mouse_coords.root.yaxis);
//		printf("mc_wdx: %d, mc_wdy: %d\n", this-> mouse_coords.wd.xaxis, this-> mouse_coords.wd.yaxis);

		while (XPending(_display) > 0) {
            XNextEvent(_display, &_xevent);
            if (_xevent.type == ClientMessage) goto end;
			if (_xevent.type == KeyPress) {
				this-> key_press = true;
				this-> key_code = _xevent.xkey.keycode;
			} else if (_xevent.type == KeyRelease) {
				this-> key_press = false;
				this-> key_code = _xevent.xkey.keycode;
			} else if (_xevent.type == ButtonPress) {
				this-> button_press = true;
				this-> button_code = _xevent.xbutton.button;
			} else if (_xevent.type == ButtonRelease) {
				this-> button_press = false;
				this-> button_code = _xevent.xbutton.button;
			}

			if (this-> key_press) printf("key press: %d\n", this-> key_code);
        }

//		if (!this-> is_wd_flag(X11_WD_DONE_DRAW)) continue;

		glDrawPixels(__wd_xaxis_len, __wd_yaxis_len, GL_RGBA, GL_UNSIGNED_BYTE, this-> pixbuff);
		glXSwapBuffers(_display, _window);

		begin = std::chrono::high_resolution_clock::now();

//		if (this-> is_wd_flag(X11_WD_WAITING)) continue;
	}
*/
	end:

	printf("closing window.\n");

	glXMakeCurrent(_display, None, NULL);
    glXDestroyContext(_display, _glx_context);
    XDestroyWindow(_display, _window);
    XCloseDisplay(_display);
    std::free(r_windows);

	this-> add_wd_flag(FLG_WD_CLOSED, true);
	this-> add_wd_flag(FLG_WD_KILLED);
	printf("x11_window: window thread killed.\n");
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::x11_window::open_in_thread(u16_t __wd_xaxis_len, u16_t __wd_yaxis_len, char const *__frame_title) {
	static boost::thread th(boost::bind(&x11_window::begin, this, __wd_xaxis_len, __wd_yaxis_len, __frame_title));
	this-> native_handle = th.native_handle();
	fprintf(stdout, "x11_window: note, waiting for window to open.\n");
	while(!this-> is_wd_flag(FLG_WD_OPEN)){
		if (this-> init_report != FFLY_SUCCESS) {
			fprintf(stderr, "x11_window: failed to open window.\n");
			return FFLY_FAILURE;
		}

		if (this-> is_wd_flag(FLG_WD_KILLED)) {
			fprintf(stderr, "x11_window: window was killed.\n");
			return FFLY_FAILURE;
		}
	}
	return FFLY_SUCCESS;
}

/*
int main() {
	mdl::firefly::graphics::x11_window x11_window;
	x11_window.begin(640, 640, "Hello World");
}*/
