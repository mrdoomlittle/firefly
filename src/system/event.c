# include "event.h"
# include "errno.h"
# include "err.h"
# include "io.h"
# include "../types/wd_event_t.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../context.h"
# include "../log.h"
void *ir_arg[20];
ff_err_t(*ir_top[20])(ffly_event_t*, void*) = {NULL};
ff_err_t(**ir_end)(ffly_event_t*, void*) = ir_top;
ff_err_t ffly_add_eir(ff_err_t(*__ir)(ffly_event_t*, void*), void *__arg_p) {
    *ir_end = __ir;
    ir_arg[ir_end-ir_top] = __arg_p;
    *(++ir_end) = NULL;
    return FFLY_SUCCESS;
}

ffly_event_t* ffly_event_dup(ffly_event_t *__event) {
    ffly_event_t *event = (ffly_event_t*)__ffly_mem_alloc(sizeof(ffly_event_t)); 
    *event = *__event;
    if (__event->field == _ffly_ef_wd) {
        ff_u8_t kind = __event->kind;
        if (kind == _ffly_wd_ek_btn_press
            || kind == _ffly_wd_ek_btn_release
            || kind == _ffly_wd_ek_key_press
            || kind == _ffly_wd_ek_key_release) {
            event->data = __ffly_mem_alloc(sizeof(ffly_event_t));
            *(ffly_wd_event_t*)event->data = *(ffly_wd_event_t*)__event->data;
        }
    }
    return event; 
}

ff_err_t ffly_event_push(ffly_event_t *__event) {
    ff_err_t(**next)(ffly_event_t*, void*) = ir_top;
    while(*next != NULL) {
        (*next)(__event, ir_arg[next-ir_top]);
        next++;
	}

	if (ffly_queue_size(&__ctx(event_queue)) > 20) {
		ffly_fprintf(ffly_err, "event queue overflow.\n");
		return FFLY_FAILURE;
	}

	ff_err_t err;
	if (_err(err = ffly_queue_push(&__ctx(event_queue), &__event))) {
		ffly_fprintf(ffly_err, "failed to push event to queue.\n");
		return FFLY_FAILURE;
	}

	ff_log("pushed event to queue.\n");
	return FFLY_SUCCESS;
}

ff_err_t ffly_event_peek(ffly_event_t **__event) {
    if (!ffly_queue_size(&__ctx(event_queue))) {
        return FFLY_FAILURE;
    }

    *__event = (ffly_event_t*)ffly_queue_front(&__ctx(event_queue));
    return FFLY_SUCCESS;
}

ff_err_t ffly_event_pop(ffly_event_t **__event) {
	ff_err_t err;
	if (_err(err = ffly_queue_pop(&__ctx(event_queue), __event))) {
		ffly_fprintf(ffly_err, "failed to pop event from queue.\n");
		return FFLY_FAILURE;
	}
	ff_log("popped event from queue.\n");
	return FFLY_SUCCESS;
}

ff_bool_t ffly_pending_event(void) {
	return ffly_queue_size(&__ctx(event_queue))>0;
}

