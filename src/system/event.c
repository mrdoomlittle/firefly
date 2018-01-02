# include "event.h"
# include "errno.h"
# include "err.h"
# include "io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
void *ir_arg[20];
ffly_err_t(*ir_top[20])(ffly_event_t*, void*) = {NULL};
ffly_err_t(**ir_end)(ffly_event_t*, void*) = ir_top;
ffly_err_t ffly_add_eir(ffly_err_t(*__ir)(ffly_event_t*, void*), void *__arg_p) {
    *ir_end = __ir;
    ir_arg[ir_end-ir_top] = __arg_p;
    *(++ir_end) = NULL;
}

struct ffly_queue ffly_event_queue;
ffly_err_t ffly_event_push(ffly_event_t *__event) {
    ffly_err_t(**next)(ffly_event_t*, void*) = ir_top;
    while(*next != NULL) {
        (*next)(__event, ir_arg[next-ir_top]);
        next++;
	}

	if (ffly_queue_size(&ffly_event_queue) > 20) {
		ffly_fprintf(ffly_err, "event queue overflow.\n");
		return FFLY_FAILURE;
	}

	ffly_err_t err;
	if (_err(err = ffly_queue_push(&ffly_event_queue, &__event))) {
		ffly_fprintf(ffly_err, "failed to push event to queue.\n");
		return FFLY_FAILURE;
	}

	ffly_fprintf(ffly_log, "pushed event to queue.\n");
	return FFLY_SUCCESS;
}

ffly_err_t ffly_event_peek(ffly_event_t **__event) {
    if (!ffly_queue_size(&ffly_event_queue)) {
        return FFLY_FAILURE;
    }

    *__event = (ffly_event_t*)ffly_queue_front(&ffly_event_queue);
    return FFLY_SUCCESS;
}

ffly_err_t ffly_event_pop(ffly_event_t **__event) {
	ffly_err_t err;
	if (_err(err = ffly_queue_pop(&ffly_event_queue, __event))) {
		ffly_fprintf(ffly_err, "failed to pop event from queue.\n");
		return FFLY_FAILURE;
	}

	ffly_fprintf(ffly_log, "poped event from queue.\n");
	return FFLY_SUCCESS;
}

ffly_bool_t ffly_pending_event() {
	return ffly_queue_size(&ffly_event_queue)>0;
}

# define FAST_EVENTS 50
ffly_event_t static *events = NULL;
ffly_event_t static *fresh_event;
ffly_event_t static **free_events = NULL;
ffly_event_t static **next_free = NULL;
ffly_event_t *ffly_alloc_event(ffly_err_t *__err) {
    *__err = FFLY_SUCCESS;
	if (events == NULL) {
		if ((events = (ffly_event_t*)__ffly_mem_alloc(FAST_EVENTS*sizeof(ffly_event_t))) == NULL) {
            *__err = FFLY_FAILURE;
            return NULL;
        }
		fresh_event = events;
	}

	ffly_fprintf(ffly_log, "alloced event.\n");
	if (next_free > free_events)
		return *(--next_free);

    if (fresh_event >= events+FAST_EVENTS) {
        return (ffly_event_t*)__ffly_mem_alloc(sizeof(ffly_event_t));
    }
	return fresh_event++;
}

ffly_err_t ffly_free_event(ffly_event_t *__event) {
    if (!__event) return FFLY_SUCCESS;
	if (free_events == NULL) {
		if ((free_events = (ffly_event_t**)__ffly_mem_alloc(FAST_EVENTS*sizeof(ffly_event_t*))) == NULL) {
            return FFLY_FAILURE;
        }
		ffly_event_t **itr = free_events;
		while(itr != free_events+FAST_EVENTS) *(itr++) = NULL;
		next_free = free_events;
	}

	ffly_fprintf(ffly_log, "freed event.\n");
    if ((__event < events || __event >= events+FAST_EVENTS) && next_free >= free_events+FAST_EVENTS) {
        __ffly_mem_free(__event);
    } else
        *(next_free++) = __event;
    return FFLY_SUCCESS;
}
