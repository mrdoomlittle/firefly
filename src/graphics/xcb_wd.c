# include "xcb_wd.h"
# include "../system/io.h"
# include "../data/mem_set.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../data/str_len.h"
ffly_err_t ffly_xcb_wd_init(struct ffly_xcb_wd *__wd, mdl_u16_t __width, mdl_u16_t __height, char const *__title) {
	ffly_mem_set(__wd, 0x0, sizeof(struct ffly_xcb_wd));
	__wd->title = __title;
	__wd->width = __width;
	__wd->height = __height;
	if ((__wd->frame_buff = __ffly_mem_alloc(__width*__height*4)) == NULL) {
		ffly_fprintf(ffly_err, "failed to allocate memory for frame buffer.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_xcb_wd_open(struct ffly_xcb_wd *__wd) {
	if (!(__wd->d = XOpenDisplay(NULL))) {
		ffly_fprintf(ffly_err, "failed to open display.\n");
		return FFLY_FAILURE;
	}

	if (!(__wd->conn = XGetXCBConnection(__wd->d))) {
		XCloseDisplay(__wd->d);
		ffly_fprintf(ffly_err, "failed to get connection.\n");
		return FFLY_FAILURE;
	}

	XSetEventQueueOwner(__wd->d, XCBOwnsEventQueue);
	xcb_setup_t *setup = xcb_get_setup(__wd->conn);

	xcb_screen_iterator_t screen_itr = xcb_setup_roots_iterator(setup);
	int def_screen = DefaultScreen(__wd->d);
	int screen_no = def_screen;
	for (;screen_itr.rem && screen_no > 0;screen_no--)
		xcb_screen_next(&screen_itr);

	__wd->screen = screen_itr.data;
	__wd->w = xcb_generate_id(__wd->conn);

	int visual;
	GLXFBConfig *fb_configs;
	int fb_config_c;
	if (!(fb_configs = glXGetFBConfigs(__wd->d, def_screen, &fb_config_c))) {
		ffly_fprintf(ffly_err, "failed to get frame buffer config.\n");
		return FFLY_FAILURE;
	}

	if (!fb_config_c) return FFLY_FAILURE;

	GLXFBConfig fb_config = *fb_configs;
	glXGetFBConfigAttrib(__wd->d, fb_config, GLX_VISUAL_ID, &visual);
	if (!(__wd->glx_ct = glXCreateNewContext(__wd->d, fb_config, GLX_RGBA_TYPE, 0, True))) {
		ffly_fprintf(ffly_err, "failed to create glx context.\n");
		return FFLY_FAILURE;
	}

	xcb_colormap_t colour_map = xcb_generate_id(__wd->conn);
	xcb_create_colormap(__wd->conn, XCB_COLORMAP_ALLOC_NONE, colour_map, __wd->screen->root, visual);
	mdl_u32_t event_mask = XCB_EVENT_MASK_EXPOSURE|XCB_EVENT_MASK_KEY_PRESS|XCB_EVENT_MASK_KEY_RELEASE|XCB_EVENT_MASK_BUTTON_PRESS|XCB_EVENT_MASK_BUTTON_RELEASE;
	mdl_u32_t values[] = {event_mask, colour_map, XCB_EVENT_MASK_EXPOSURE, 0};
	mdl_u32_t mask = XCB_CW_EVENT_MASK|XCB_CW_COLORMAP;

	xcb_create_window(__wd->conn, __wd->screen->root_depth, __wd->w, __wd->screen->root, 0, 0, __wd->width, __wd->height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, visual, mask, values);

	xcb_change_property(__wd->conn, XCB_PROP_MODE_REPLACE, __wd->w, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, ffly_str_len(__wd->title), __wd->title);
/* de comment later
	// size hints
	xcb_size_hints_t size_hints;
	xcb_icccm_size_hints_set_min_size(&size_hints, __wd->mn_width, __wd->mn_height);
	xcb_icccm_size_hints_set_max_size(&size_hints, __wd->mx_width, __wd->mx_height);
	xcb_icccm_set_wm_size_hints(__wd->conn, __wd->w, XCB_ATOM_WM_NORMAL_HINTS, &size_hints);
*/
	xcb_intern_atom_cookie_t proto_cookie = xcb_intern_atom(__wd->conn, 1, 12, "WM_PROTOCOLS");
	xcb_intern_atom_reply_t *proto_re = xcb_intern_atom_reply(__wd->conn, proto_cookie, 0);

	xcb_intern_atom_cookie_t cookie = xcb_intern_atom(__wd->conn, 0, 16, "WM_DELETE_WINDOW");
 	xcb_intern_atom_reply_t *re = xcb_intern_atom_reply(__wd->conn, cookie, 0);

	xcb_change_property(__wd->conn, XCB_PROP_MODE_REPLACE, __wd->w, proto_re->atom, 4, 32, 1, &re->atom);

	if (!(__wd->glx_window = glXCreateWindow(__wd->d, fb_config, __wd->w, NULL))) {
		return FFLY_FAILURE;
	}

	__wd->glx_dr = __wd->glx_window;
	if(!glXMakeContextCurrent(__wd->d, __wd->glx_dr, __wd->glx_dr, __wd->glx_ct)) {
		return FFLY_FAILURE;
	}

	xcb_map_window(__wd->conn, __wd->w);
	xcb_flush(__wd->conn);
	return FFLY_SUCCESS;
}

ffly_err_t ffly_xcb_wd_close(struct ffly_xcb_wd *__wd) {
	xcb_destroy_window(__wd->conn, __wd->w);
	glXDestroyContext(__wd->d, __wd->glx_ct);
	glXDestroyWindow(__wd->d, __wd->glx_window);
	XCloseDisplay(__wd->d);
	return FFLY_SUCCESS;
}

ffly_err_t ffly_xcb_wd_cleanup(struct ffly_xcb_wd *__wd) {
	__ffly_mem_free(__wd->frame_buff);
	return FFLY_SUCCESS;
}
