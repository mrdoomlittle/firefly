# include "xcb_window.hpp"
/* change uint_t to boost::uint16_t as it seems like we dont need to use uint_t
*/
boost::int8_t mdl::firefly::graphics::xcb_window::init(boost::uint16_t __wd_xaxis_len, boost::uint16_t __wd_yaxis_len, char const *__frame_title) {
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

	xcb_screen_t *screen = 0;
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
	boost::uint32_t eventmask = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
	boost::uint32_t valuelist[] = { eventmask, colormap, 0 };
	boost::uint32_t valuemask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;

	xcb_create_window(conn, XCB_COPY_FROM_PARENT, window, screen-> root, 0, 0, __wd_xaxis_len, __wd_yaxis_len, 0,
	XCB_WINDOW_CLASS_INPUT_OUTPUT, vis_id, valuemask, valuelist);

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

	do {
		xcb_generic_event_t *event = xcb_wait_for_event(conn);
		if (!event) {
			return FFLY_FAILURE;
		}

		switch(event-> response_type & ~0x80) {
			case XCB_KEY_PRESS:
				std::free(event);
				return 0;
			break;

			case XCB_EXPOSE:
			break;
		}

	} while(1);

	xcb_destroy_window(conn, window);
	glXDestroyWindow(display, glx_window);

	XCloseDisplay(display);

	return FFLY_SUCCESS;
}
// NOTE: this is not finished
int main() {
	mdl::firefly::graphics::xcb_window xcb_window;
	xcb_window.init(640, 640, "\0");
}
