# include "window.h"
# include "../system/err.h"
# include "../system/io.h"
# include "../types/wd_ed_t.h"
# include "wd_flags.h"
# include "../system/nanosleep.h"
# include "../data/mem_set.h"
# include "../system/event_kind.h"
# include "../system/event_field.h"
# include "../system/event.h"
# include "../types/wd_event_t.h"
ffly_byte_t* ffly_wd_frame_buff(struct ffly_wd *__wd) {
	return __wd->raw.frame_buff;
}
ffly_flag_t* ffly_wd_flags(struct ffly_wd *__wd) {
	return &__wd->flags;
}

ffly_err_t ffly_wd_open(struct ffly_wd *__wd) {
	ffly_err_t err;
# ifdef __ffly_use_x11
	err = ffly_x11_wd_open(&__wd->raw);
# elif __ffly_use_xcb
	err = ffly_xcb_wd_open(&__wd->raw);
# endif
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to open window.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_wd_close(struct ffly_wd *__wd) {
	ffly_err_t err;
# ifdef __ffly_use_x11
	err = ffly_x11_wd_close(&__wd->raw);
# elif __ffly_use_xcb
	err = ffly_xcb_wd_close(&__wd->raw);
# endif
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to close window.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_wd_init(struct ffly_wd *__wd, mdl_u16_t __width, mdl_u16_t __height, char const *__title) {
	ffly_mem_set(__wd, 0x0, sizeof(struct ffly_wd));
	ffly_err_t err;
# ifdef __ffly_use_x11
	err = ffly_x11_wd_init(&__wd->raw, __width, __height, __title);
# elif __ffly_use_xcb
	err = ffly_xcb_wd_init(&__wd->raw, __width, __height, __title);
# endif
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failure to init window.\n");
		return err;
	}

	ffly_mem_blk_init(&__wd->events, 20, sizeof(ffly_wd_event_t));
	return FFLY_SUCCESS;
}

ffly_err_t ffly_wd_cleanup(struct ffly_wd *__wd) {
	ffly_err_t err;
# ifdef __ffly_use_x11
	err = ffly_x11_wd_cleanup(&__wd->raw);
# elif __ffly_use_xcb
	err = ffly_xcb_wd_cleanup(&__wd->raw);
# endif
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to cleanup window.\n");
		return err;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_wd_display(struct ffly_wd *__wd) {
	glDrawPixels(__wd->raw.width, __wd->raw.height, GL_RGBA, GL_UNSIGNED_BYTE, __wd->raw.frame_buff);
# ifdef __ffly_use_x11
	glXSwapBuffers(__wd->raw.d, __wd->raw.w);
# elif __ffly_use_xcb
	glXSwapBuffers(__wd->raw.d, __wd->raw.glx_dr);
# endif
}

void static mk_event(ffly_event_t **__event, ffly_event_t __tmpl) {
	**__event = __tmpl;
	(*__event)->field = _ffly_ef_wd;
}

# ifdef __ffly_use_x11
ffly_err_t ffly_x11_wd_poll_event(struct ffly_wd *__wd, ffly_event_t **__event) {
	XEvent event;
	if (XPending(__wd->raw.d) > 0) {
		*__event = ffly_alloc_event();
		XNextEvent(__wd->raw.d, &event);
		switch(event.type) {
			case ClientMessage:
				mk_event(__event, (ffly_event_t){.kind=_ffly_wd_ek_closed});
			break;
			case KeyPress:
				mk_event(__event, (ffly_event_t){.kind=_ffly_wd_ek_key_press, .data=ffly_mem_blk_alloc(&__wd->events)});
				*((ffly_wd_event_t*)(*__event)->data) = (ffly_wd_event_t){
					.code = event.xkey.keycode
				};
			break;
			case KeyRelease:
				mk_event(__event, (ffly_event_t){.kind=_ffly_wd_ek_key_release, .data=ffly_mem_blk_alloc(&__wd->events)});
				*((ffly_wd_event_t*)(*__event)->data) = (ffly_wd_event_t){
					.code = event.xkey.keycode
				};
			break;
			default:
				mk_event(__event, (ffly_event_t){.kind=_ffly_ek_unknown});
		}
	}
	return FFLY_SUCCESS;
}
# endif
# ifdef __ffly_use_xcb
ffly_err_t ffly_xcb_wd_poll_event(struct ffly_wd *__wd, ffly_event_t **__event) {
	xcb_generic_event_t *event;
	if ((event = xcb_poll_for_event(__wd->raw.conn)) != NULL) {
		*__event = ffly_alloc_event();
		switch(event->response_type&~0x80) {
			case XCB_CLIENT_MESSAGE:
				mk_event(__event, (ffly_event_t){.kind=_ffly_wd_ek_closed});
			break;
			default:
				mk_event(__event, (ffly_event_t){.kind=_ffly_ek_unknown});
		}
	}

	free(event);
	return FFLY_SUCCESS;
}
# endif

ffly_event_t* ffly_wd_poll_event(struct ffly_wd *__wd, ffly_err_t *__err) {
	ffly_event_t *event = NULL;
# ifdef __ffly_use_x11
	*__err = ffly_x11_wd_poll_event(__wd, &event);
# elif __ffly_use_xcb
	*__err = ffly_xcb_wd_poll_event(__wd, &event);
# endif
	if (_err(*__err)) {
		ffly_fprintf(ffly_err, "window failed.\n");
		return NULL;
	}
	*__err = FFLY_SUCCESS;
	return event;
}

ffly_err_t ffly_wd_free_event(struct ffly_wd *__wd, ffly_event_t *__event) {
	if (__event->kind  == _ffly_wd_ek_key_press || __event->kind == _ffly_wd_ek_key_release) {
		ffly_mem_blk_free(&__wd->events, __event->data);
	}
}
