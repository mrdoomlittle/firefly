# include "xcb_wd.h"
ffly_err_t ffly_xcb_wd_begin(struct ffly_xcb_wd *__xcb_wd, mdl_uint_t __xa_len, mdl_uint_t __ya_len, char const *__title) {
	if (!(__xcb_wd->d = XOpenDisplay(NULL))) {
		return FFLY_FAILURE;
	}

	if (!(__xcb_wd->conn = XGetXCBConnection(__xcb_wd->d))) {
		XCloseDisplay(__xcb_wd->d);
		return FFLY_FAILURE;
	}

	XSetEventQueueOwner(__xcb_wd->d, XCBOwnsEventQueue);
	xcb_setup_t const *setup = xcb_get_setup(__xcb_wd->conn);

	xcb_screen_iterator_t screen_itr = xcb_setup_roots_iterator(setup);
	int def_screen = DefaultScreen(__xcb_wd->d);
	mdl_uint_t ic;
	for (ic = 0; ic < def_screen; ++ic)
		xcb_screen_next(&screen_itr);

	__xcb_wd->screen = screen_itr.data;
	__xcb_wd->w = xcb_generate_id(__xcb_wd->conn);

	GLXContext glx_ct;
	int vis_id;
	GLXFBConfig *fb_configs;
	int fb_config_c;
	if (!(fb_configs = glXGetFBConfigs(__xcb_wd->d, def_screen, &fb_config_c))) {
		return FFLY_FAILURE;
	}

	if (!fb_config_c) return FFLY_FAILURE;

	GLXFBConfig fb_config = *fb_configs;
	glXGetFBConfigAttrib(__xcb_wd->d, fb_config, GLX_VISUAL_ID, &vis_id);
	if (!(glx_ct = glXCreateNewContext(__xcb_wd->d, fb_config, GLX_RGBA_TYPE, 0, True))) {

		return FFLY_FAILURE;
	}

	xcb_colormap_t colour_map = xcb_generate_id(__xcb_wd->conn);
	xcb_create_colormap(__xcb_wd->conn, XCB_COLORMAP_ALLOC_NONE, colour_map, __xcb_wd->screen->root, vis_id);
	mdl_u32_t event_msk = XCB_EVENT_MASK_EXPOSURE|XCB_EVENT_MASK_KEY_PRESS|XCB_EVENT_MASK_KEY_RELEASE|XCB_EVENT_MASK_BUTTON_PRESS|XCB_EVENT_MASK_BUTTON_RELEASE;
	mdl_u32_t val_list[] = {event_msk, colour_map, 0};
	mdl_u32_t val_msk = XCB_CW_EVENT_MASK|XCB_CW_COLORMAP;

	xcb_create_window(__xcb_wd->conn, XCB_COPY_FROM_PARENT, __xcb_wd->w, __xcb_wd->screen->root, 0, 0, __xa_len, __ya_len, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, vis_id, val_msk, val_list);
	xcb_size_hints_t size_hints;
	xcb_icccm_size_hints_set_min_size(&size_hints, __xa_len-__xcb_wd->mn_xal_off, __ya_len-__xcb_wd->mn_yal_off);
	xcb_icccm_size_hints_set_max_size(&size_hints, __xa_len+__xcb_wd->mx_xal_off, __ya_len+__xcb_wd->mx_yal_off);
	xcb_icccm_set_wm_size_hints(__xcb_wd->conn, __xcb_wd->w, XCB_ATOM_WM_NORMAL_HINTS, &size_hints);

	xcb_intern_atom_cookie_t proto_cookie = xcb_intern_atom(__xcb_wd->conn, 1, 12, "WM_PROTOCOLS");
	xcb_intern_atom_reply_t *proto_re = xcb_intern_atom_reply(__xcb_wd->conn, proto_cookie, 0);

	xcb_intern_atom_cookie_t cookie = xcb_intern_atom(__xcb_wd->conn, 0, 16, "WM_DELETE_WINDOW");
	xcb_intern_atom_reply_t *re = xcb_intern_atom_reply(__xcb_wd->conn, cookie, 0);

	xcb_change_property(__xcb_wd->conn, XCB_PROP_MODE_REPLACE, __xcb_wd->w, proto_re->atom, 4, 32, 1, &re->atom);

	xcb_map_window(__xcb_wd->conn, __xcb_wd->w);
	GLXWindow glx_window;
	if (!(glx_window = glXCreateWindow(__xcb_wd->d, fb_config, __xcb_wd->w, NULL))) {
		xcb_destroy_window(__xcb_wd->conn, __xcb_wd->w);
		glXDestroyContext(__xcb_wd->d, glx_ct);
		return FFLY_FAILURE;
	}

	GLXDrawable dr = glx_window;
	if(!glXMakeContextCurrent(__xcb_wd->d, dr, dr, glx_ct)) {
		xcb_destroy_window(__xcb_wd->conn, __xcb_wd->w);
		glXDestroyContext(__xcb_wd->d, glx_ct);
		glXDestroyWindow(__xcb_wd->d, glx_window);
		return FFLY_FAILURE;
	}

	do {
		xcb_generic_event_t *ev;
		while((ev = xcb_poll_for_event(__xcb_wd->conn)) != NULL) {
			switch(ev->response_type & ~0x80) {
				case XCB_CLIENT_MESSAGE: goto end;
			}
		}
	} while(1);

	end:
	xcb_destroy_window(__xcb_wd->conn, __xcb_wd->w);
	glXDestroyContext(__xcb_wd->d, glx_ct);
	glXDestroyWindow(__xcb_wd->d, glx_window);
	return FFLY_SUCCESS;
}
