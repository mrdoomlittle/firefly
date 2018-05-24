# include "window.h"
# include "../system/err.h"
# include "../system/io.h"
# include "../types/wd_ed_t.h"
# include "wd_flags.h"
# include "../system/nanosleep.h"
# include "../dep/mem_set.h"
# include "../types/wd_event_t.h"
ff_byte_t* ffly_wd_frame_buff(struct ffly_wd *__wd) {
	return __wd->raw.frame_buff;
}
ff_flag_t* ffly_wd_flags(struct ffly_wd *__wd) {
	return &__wd->flags;
}

ff_err_t ffly_wd_query_pointer(struct ffly_wd *__wd, ff_i16_t *__root_xa, ff_i16_t *__root_ya, ff_i16_t *__wd_xa, ff_i16_t *__wd_ya) {
    ff_err_t err;
# ifdef __ffly_use_x11
    err = ffly_x11_query_pointer(&__wd->raw, __root_xa, __root_ya, __wd_xa, __wd_ya);
# endif
# ifdef __ffly_use_xcb 
    err = ffly_xcb_query_pointer(&__wd->raw, __root_xa, __root_ya, __wd_xa, __wd_ya);
# endif
    if (_err(err)) {
        ffly_fprintf(ffly_err, "failed to query pointer.\n");
        return err;
    }
    return FFLY_SUCCESS;
}

ff_err_t ffly_wd_open(struct ffly_wd *__wd) {
	ff_err_t err;
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

ff_err_t ffly_wd_close(struct ffly_wd *__wd) {
	ff_err_t err;
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

ff_err_t ffly_wd_init(struct ffly_wd *__wd, ff_u16_t __width, ff_u16_t __height, char const *__title) {
	ffly_mem_set(__wd, 0x0, sizeof(struct ffly_wd));
	ff_err_t err;
# ifdef __ffly_use_x11
	err = ffly_x11_wd_init(&__wd->raw, __width, __height, __title);
# elif __ffly_use_xcb
	err = ffly_xcb_wd_init(&__wd->raw, __width, __height, __title);
# endif
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failure to init window.\n");
		return err;
	}

	ffly_pool_init(&__wd->events, 20, sizeof(ffly_wd_event_t));
	return FFLY_SUCCESS;
}

ff_err_t ffly_wd_cleanup(struct ffly_wd *__wd) {
	ff_err_t err;
# ifdef __ffly_use_x11
	err = ffly_x11_wd_cleanup(&__wd->raw);
# elif __ffly_use_xcb
	err = ffly_xcb_wd_cleanup(&__wd->raw);
# endif
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to cleanup window.\n");
		return err;
	}
    ffly_pool_de_init(&__wd->events);
	return FFLY_SUCCESS;
}

ff_err_t ffly_wd_display(struct ffly_wd *__wd) {
	glDrawPixels(__wd->raw.width, __wd->raw.height, GL_RGBA, GL_UNSIGNED_BYTE, __wd->raw.frame_buff);
# ifdef __ffly_use_x11
	glXSwapBuffers(__wd->raw.d, __wd->raw.w);
# elif __ffly_use_xcb
	glXSwapBuffers(__wd->raw.d, __wd->raw.glx_dr);
# endif
}

void static
mk_event(ffly_event_t **__event, ffly_event_t __tmpl) {
	**__event = __tmpl;
	(*__event)->field = _ffly_ef_wd;
}

# ifdef __ffly_use_x11
ff_err_t 
ffly_x11_wd_poll_event(struct ffly_wd *__wd, ffly_event_t **__event) {
	ff_err_t err;
	XEvent event;
	if (XPending(__wd->raw.d) > 0) {
		*__event = ff_event_alloc(&err);
		if (_err(err)) {
			ffly_fprintf(ffly_err, "failed to allocate event.\n");
			return err;
		}
		XNextEvent(__wd->raw.d, &event);
		switch(event.type) {
			case ClientMessage:
				mk_event(__event, (ffly_event_t){.kind=_ffly_wd_ek_closed});
			break;
            case ButtonPress: case ButtonRelease:
                mk_event(__event, (ffly_event_t){.kind=event.type == ButtonPress?_ffly_wd_ek_btn_press:_ffly_wd_ek_btn_release, .data=ffly_pool_alloc(&__wd->events), .size=sizeof(ffly_wd_event_t)});
                *((ffly_wd_event_t*)(*__event)->data) = (ffly_wd_event_t){
                    .btn = ffly_x11_convert_btnno(event.xbutton.button),
                    .x = event.xbutton.x,
                    .y = event.xbutton.y
                };
            break;
			case KeyPress: case KeyRelease:
				mk_event(__event, (ffly_event_t){.kind=event.type == KeyPress?_ffly_wd_ek_key_press:_ffly_wd_ek_key_release, .data=ffly_pool_alloc(&__wd->events), .size=sizeof(ffly_wd_event_t)});
				*((ffly_wd_event_t*)(*__event)->data) = (ffly_wd_event_t){
                    .keycode = ffly_x11_convert_keycode(event.xkey.keycode),
                    .x = event.xkey.x,
                    .y = event.xkey.y
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
ff_err_t
ffly_xcb_wd_poll_event(struct ffly_wd *__wd, ffly_event_t **__event) {
    ff_err_t err;
	xcb_generic_event_t *event;
	if ((event = xcb_poll_for_event(__wd->raw.conn)) != NULL) {
		*__event = ff_event_alloc(&err);
        if (_err(err)) {
            free(event);
            ffly_fprintf(ffly_err, "failed to allocate event.\n");
            return err;
        }
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

ffly_event_t*
ffly_wd_poll_event(struct ffly_wd *__wd, ff_err_t *__err) {
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

ff_err_t ffly_wd_free_event(struct ffly_wd *__wd, ffly_event_t *__event) {
	if (__event->kind  == _ffly_wd_ek_key_press || __event->kind == _ffly_wd_ek_key_release
        || __event->kind == _ffly_wd_ek_btn_press || __event->kind == _ffly_wd_ek_btn_release) {
		ffly_pool_free(&__wd->events, __event->data);
	}
}
