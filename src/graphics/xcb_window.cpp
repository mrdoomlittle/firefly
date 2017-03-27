# include "xcb_window.hpp"
/* change uint_t to boost::uint16_t as it seems like we dont need to use uint_t
*/
boost::int8_t mdl::firefly::graphics::xcb_window::begin(boost::uint16_t __wd_xaxis_len, boost::uint16_t __wd_yaxis_len, char const *__frame_title) {
	printf("thread has started.\n");
	if (this-> is_wd_flag(FLG_WD_KILLED)) return FFLY_SUCCESS;

	this-> wd_xaxis_len = __wd_xaxis_len;
	this-> wd_yaxis_len = __wd_yaxis_len;
	this-> frame_title = (char *)memory::mem_alloc(strlen(__frame_title));
	if (this-> frame_title == NULL) {
		fprintf(stderr, "xcb_window: failed to alloc memory for replacment frame title, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	memcpy(this-> frame_title, __frame_title, strlen(__frame_title));

	this-> pixbuff = memory::alloc_pixmap(__wd_xaxis_len, __wd_yaxis_len, 1);
	if (this-> pixbuff == NULL) {
		fprintf(stderr, "xcb_window: failed to alloc memory for pixbuff, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	bzero(this-> pixbuff, (__wd_xaxis_len * __wd_yaxis_len) * 4);

	printf("creating window with size %dx%d and a title of '%s'\n", __wd_yaxis_len, __wd_xaxis_len, __frame_title);

	Display *display;
	int def_screen;

	display = XOpenDisplay(0);
	if (!display) {
		fprintf(stderr, "failed to open x display.\n");
		return FFLY_FAILURE;
	}

	def_screen = DefaultScreen(display);
	xcb_connection_t *conn = XGetXCBConnection(display);
	if (!conn) {
		XCloseDisplay(display);
		fprintf(stderr, "failed to get xcb conn.\n");
		return FFLY_FAILURE;
	}

	XSetEventQueueOwner(display, XCBOwnsEventQueue);

	xcb_screen_t *screen = NULL;
	xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(conn));

	for(int o = def_screen; screen_iter.rem && o > 0; --o) {
		xcb_screen_next(&screen_iter);
	}

	screen = screen_iter.data;

	GLXContext context;

	int vis_id = 0;
	GLXFBConfig *fb_configs = 0;
	int nfb_configs = 0;

	fb_configs = glXGetFBConfigs(display, def_screen, &nfb_configs);
	if (!fb_configs || nfb_configs == 0) {
		return FFLY_FAILURE;
	}

	GLXFBConfig fb_config = fb_configs[0];
	glXGetFBConfigAttrib(display, fb_config, GLX_VISUAL_ID , &vis_id);

	if (!(context = glXCreateNewContext(display, fb_config, GLX_RGBA_TYPE, 0, True))) {
		return FFLY_FAILURE;
	}

	xcb_colormap_t colormap = xcb_generate_id(conn);
	xcb_window_t window = xcb_generate_id(conn);

	xcb_create_colormap(conn, XCB_COLORMAP_ALLOC_NONE, colormap, screen-> root, vis_id);
	boost::uint32_t eventmask = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE;
	boost::uint32_t valuelist[] = { eventmask, colormap, 0 };
	boost::uint32_t valuemask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;

	xcb_create_window(conn, XCB_COPY_FROM_PARENT, window, screen-> root, 0, 0, __wd_xaxis_len, __wd_yaxis_len, 0,
	XCB_WINDOW_CLASS_INPUT_OUTPUT, vis_id, valuemask, valuelist);

	xcb_size_hints_t shints;
	xcb_icccm_size_hints_set_min_size(&shints, __wd_xaxis_len - 1, __wd_yaxis_len - 1);
	xcb_icccm_size_hints_set_max_size(&shints, __wd_xaxis_len, __wd_yaxis_len);

	xcb_icccm_set_wm_size_hints(conn, window, XCB_ATOM_WM_NORMAL_HINTS, &shints);

	xcb_change_property(conn, XCB_PROP_MODE_REPLACE, window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(__frame_title), __frame_title);

	xcb_intern_atom_cookie_t cook = xcb_intern_atom(conn, 1, 12, "WM_PROTOCOLS");
	xcb_intern_atom_reply_t* rep = xcb_intern_atom_reply(conn, cook, 0);

	xcb_intern_atom_cookie_t cook2 = xcb_intern_atom(conn, 0, 16, "WM_DELETE_WINDOW");
	xcb_intern_atom_reply_t* rep2 = xcb_intern_atom_reply(conn, cook2, 0);

	xcb_change_property(conn, XCB_PROP_MODE_REPLACE, window, rep-> atom, 4, 32, 1, &rep2-> atom);

	xcb_map_window(conn, window);
	GLXWindow glx_window = glXCreateWindow(display, fb_config, window, NULL);
	if (!window) {
		xcb_destroy_window(conn, window);
		glXDestroyContext(display, context);
		return FFLY_FAILURE;
	}

	GLXDrawable drawable = glx_window;

	if(!glXMakeContextCurrent(display, drawable, drawable, context)) {
		xcb_destroy_window(conn, window);
		glXDestroyContext(display, context);
		return FFLY_FAILURE;
	}

	this-> add_wd_flag(FLG_WD_OPEN, true);
	system::stop_watch fps_timer;
	fps_timer.begin();

	uint_t time_each_frame = 100000000/this-> fps_mark;
	do {
		xcb_generic_error_t *any_error;

		xcb_query_pointer_cookie_t qp_cookie = xcb_query_pointer(conn, window);
		xcb_query_pointer_reply_t *qp_reply = xcb_query_pointer_reply(conn, qp_cookie, &any_error);
		if (any_error) {
			fprintf(stderr, "xcb_window: failed to get reply for pointer query.\n");
			break;
		}

		xcb_translate_coordinates_cookie_t tc_cookie = xcb_translate_coordinates(conn, window, screen-> root, 0, 0);
		xcb_translate_coordinates_reply_t *tc_reply = xcb_translate_coordinates_reply(conn, tc_cookie, &any_error);
		if (any_error) {
			fprintf(stderr, "xcb_window: failed to get reply for coordinates.\n");
			break;
		}

		this-> wd_coords.xaxis = tc_reply-> dst_x;
		this-> wd_coords.yaxis = tc_reply-> dst_y;

		this-> mouse_coords.root.xaxis = qp_reply-> root_x;
		this-> mouse_coords.root.yaxis = qp_reply-> root_y;

		std::free(qp_reply);
		std::free(tc_reply);

		if ((this-> mouse_coords.root.xaxis > this-> wd_coords.xaxis && this-> mouse_coords.root.xaxis < (this-> wd_coords.xaxis + __wd_xaxis_len))
			&& (this-> mouse_coords.root.yaxis > this-> wd_coords.yaxis && this-> mouse_coords.root.yaxis < (this-> wd_coords.yaxis + __wd_yaxis_len))) {
			this-> mouse_coords.wd.xaxis = (this-> mouse_coords.root.xaxis - this-> wd_coords.xaxis);
			this-> mouse_coords.wd.yaxis = (this-> mouse_coords.root.yaxis - this-> wd_coords.yaxis);

			this-> contains_pointer = true;
		} else
			this-> contains_pointer = false;

		xcb_generic_event_t *event;
		while ((event = xcb_poll_for_event(conn)) != NULL) {
			switch(event-> response_type & ~0x80) {
				case XCB_CLIENT_MESSAGE: {
					goto end;
				}

				case XCB_KEY_PRESS: {
					xcb_key_press_event_t *_kp_event = (xcb_key_press_event_t *)event;
					this-> key_press = true;
					this-> key_code = _kp_event-> detail;
				break;
				}

				case XCB_KEY_RELEASE: {
					//xcb_key_release_event_t *_event = (xcb_key_release_event_t *)event;
					this-> key_press = false;
				break;
				}

				case XCB_BUTTON_PRESS: {
					xcb_button_press_event_t *_bp_event = (xcb_button_press_event_t *)event;
					this-> button_press = true;
					this-> key_code = _bp_event-> detail;
				break;
				}

				case XCB_BUTTON_RELEASE: {
					//xcb_button_release_event_t *_event = (xcb_button_release_event_t *)event;
					this-> button_press = false;
				break;
				}
			}
		}

		fps_timer.time_point();
		if (fps_timer.get_delta<std::chrono::nanoseconds>() < time_each_frame) break;

		this-> add_wd_flag(FLG_WD_WAITING);

		if (!this-> is_wd_flag(FLG_WD_DONE_DRAW)) continue;

		glDrawPixels(__wd_xaxis_len, __wd_yaxis_len, GL_RGBA, GL_UNSIGNED_BYTE, this-> pixbuff);
		glXSwapBuffers(display, drawable);

		this-> rm_wd_flag(FLG_WD_DONE_DRAW);

		if (this-> is_wd_flag(FLG_WD_WAITING)) continue;

		fps_timer.begin();
	} while(!this-> is_wd_flag(FLG_WD_TO_CLOSE));
	end:

	printf("closed.\n");

	xcb_destroy_window(conn, window);
	glXDestroyContext(display, context);
	glXDestroyWindow(display, glx_window);
	XCloseDisplay(display);

	this-> add_wd_flag(FLG_WD_CLOSED, true);
	this-> add_wd_flag(FLG_WD_KILLED);
	printf("thread has ended.\n");
	return FFLY_SUCCESS;
}

boost::int8_t mdl::firefly::graphics::xcb_window::open_in_thread(boost::uint16_t __wd_xaxis_len, boost::uint16_t __wd_yaxis_len, char const *__frame_title) {
	boost::thread th(boost::bind(&xcb_window::begin, this, __wd_xaxis_len, __wd_yaxis_len, __frame_title));
	this-> native_handle = th.native_handle();
	return FFLY_SUCCESS;
}

// NOTE: this is not finished
/*
int main() {
	mdl::firefly::graphics::xcb_window xcb_window;
	xcb_window.begin(640, 640, "\0");
}
*/
