# include "event.h"
# include "errno.h"
# include "err.h"
# include "io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
struct ffly_queue ffly_event_queue;
ffly_err_t ffly_event_push(ffly_event_t *__event) {
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

# define MAX_EVENTS 50
ffly_event_t static *events = NULL;
ffly_event_t static *fresh_event;
ffly_event_t static **free_events = NULL;
ffly_event_t static **next_free = NULL;

ffly_event_t *ffly_alloc_event() {
	if (events == NULL) {
		events = (ffly_event_t*)__ffly_mem_alloc(sizeof(ffly_event_t));
		fresh_event = events;
	}

	ffly_fprintf(ffly_log, "alloced event.\n");
	if (next_free > free_events)
		return *(--next_free);

    if (fresh_event > events+MAX_EVENTS)
        return NULL;
	return fresh_event++;
}

void ffly_free_event(ffly_event_t *__event) {
	if (free_events == NULL) {
		free_events = (ffly_event_t**)__ffly_mem_alloc(MAX_EVENTS*sizeof(ffly_event_t*));
		ffly_event_t **itr = free_events;
		while(itr != free_events+MAX_EVENTS) *(itr++) = NULL;
		next_free = free_events;
	}

	ffly_fprintf(ffly_log, "freed event.\n");
	*(next_free++) = __event;
}
