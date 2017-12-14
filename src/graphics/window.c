# include "window.h"
# include "../system/err.h"
# include "../system/io.h"
# include "../types/wd_ed_t.h"
# include "wd_flags.h"
# include "../system/nanosleep.h"
# include "../types/event_t.h"
# include "../data/mem_set.h"
ffly_byte_t* ffly_wd_frame_buff(struct ffly_wd *__wd) {
	return __wd->raw.frame_buff;
}
ffly_flag_t* ffly_wd_flags(struct ffly_wd *__wd) {
	return &__wd->flags;
}

ffly_err_t ffly_wd_open(struct ffly_wd *__wd) {
	ffly_err_t err;
# ifdef __USING_X11
	err = ffly_x11_wd_open(&__wd->raw);
# elif __USING_XCB
	err = ffly_xcb_wd_open(&__wd->raw);
# endif
	if (_err(err)) {
		ffly_printf(stderr, "failed to open window.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_wd_close(struct ffly_wd *__wd) {
	ffly_err_t err;
# ifdef __USING_X11
	err = ffly_x11_wd_close(&__wd->raw);
# elif __USING_XCB
	err = ffly_xcb_wd_close(&__wd->raw);
# endif
	if (_err(err)) {
		ffly_printf(stderr, "failed to close window.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_wd_init(struct ffly_wd *__wd, mdl_u16_t __width, mdl_u16_t __height, char const *__title) {
	ffly_mem_set(__wd, 0x0, sizeof(struct ffly_wd));
	ffly_err_t err;
# ifdef __USING_X11
	err = ffly_x11_wd_init(&__wd->raw, __width, __height, __title);
# elif __USING_XCB
	err = ffly_xcb_wd_init(&__wd->raw, __width, __height, __title);
# endif
	if (_err(err)) {
		ffly_printf(stderr, "failure to init window.\n");
		return err;
	}

	if (ffly_buff_init(&__wd->event_buff, EVENT_BUFF_SIZE, sizeof(ffly_event_t)) != FFLY_SUCCESS) {
		ffly_printf(stderr, "failed to init event buffer.\n");
		return FFLY_FAILURE;
	}

	ffly_mem_blk_init(&__wd->event_data, 100, sizeof(ffly_wd_ed_t));
	return FFLY_SUCCESS;
}

ffly_err_t ffly_wd_cleanup(struct ffly_wd *__wd) {
	ffly_err_t err;
# ifdef __USING_X11
	err = ffly_x11_wd_cleanup(&__wd->raw);
# elif __USING_XCB
	err = ffly_xcb_wd_cleanup(&__wd->raw);
# endif
	if (_err(err)) {
		ffly_printf(stderr, "failed to cleanup window.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_wd_display(struct ffly_wd *__wd) {
	glDrawPixels(__wd->raw.width, __wd->raw.height, GL_RGBA, GL_UNSIGNED_BYTE, __wd->raw.frame_buff);
	glXSwapBuffers(__wd->raw.d, __wd->raw.glx_dr);
}

# ifdef __USING_X11
ffly_err_t ffly_x11_wd_begin(struct ffly_wd *__wd) {
	ffly_add_flag(&__wd->flags, FF_FLG_WD_ALIVE, 0);

	XEvent xevent;
	ffly_event_t event;
	ffly_wd_ed_t event_data;
	do {
		ffly_nanosleep(0, 1000000);
		while (XPending(__wd->raw.d) > 0) {
			XNextEvent(__wd->raw.d, &xevent);
			switch(xevent.type) {
				case ClientMessage: goto _end;
			}
		}
	} while(!ffly_is_flag(__wd->flags, FF_FLG_WD_KILL));

	_end:
	ffly_add_flag(&__wd->flags, FF_FLG_WD_DEAD, 0);
	while(!ffly_is_flag(__wd->flags, FF_FLG_WD_OK));
	return FFLY_SUCCESS;
}
# endif

# ifdef __USING_XCB
ffly_err_t ffly_xcb_wd_begin(struct ffly_wd *__wd) {
	ffly_add_flag(&__wd->flags, FF_FLG_WD_ALIVE, 0);
	xcb_generic_event_t *event;
	do {
		ffly_nanosleep(0, 1000000);
		while((event = xcb_poll_for_event(__wd->raw.conn)) != NULL) {
			switch(event->response_type&~0x80) {
				case XCB_CLIENT_MESSAGE: goto _end;
			}
		}

		free(event);
	} while(!ffly_is_flag(__wd->flags, FF_FLG_WD_KILL));
	_end:
	ffly_add_flag(&__wd->flags, FF_FLG_WD_DEAD, 0);
	while(!ffly_is_flag(__wd->flags, FF_FLG_WD_OK));
	return FFLY_SUCCESS;
}
# endif

ffly_err_t ffly_wd_begin(struct ffly_wd *__wd) {
	ffly_err_t err;
# ifdef __USING_X11
	err = ffly_x11_wd_begin(__wd);
# elif __USING_XCB
	err = ffly_xcb_wd_begin(__wd);
# endif
	if (_err(err)) {
		ffly_printf(stderr, "window failed.\n");
		return err;
	}
	return FFLY_SUCCESS;
}
