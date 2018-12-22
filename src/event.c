# include "event.h"
# include "system/errno.h"
# include "system/error.h"
# include "system/io.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/event.h"
ff_eventp
ff_event_creat(ff_u8_t __kind, ff_u8_t __field, void *__data,
	ff_uint_t __size, ff_err_t *__err)
{
	ff_eventp ret = ff_event_alloc(__err);
	ff_event_build(__kind, __field, __data, __size, ret);
	return ret;
}

ff_err_t ff_event_del(ff_eventp __event) {
	if (__event->data != NULL) {
		ffly_fprintf(ffly_log, "event data freed.\n");
		__ffly_mem_free(__event->data);
	}
	ff_event_free(__event);
}

ff_i8_t ff_event_poll(ff_eventp *__event) {
	if (!ffly_pending_event()) return -1;
	ffly_event_pop(__event);
	return 0;
}

#define FAST_EVENTS 50
ffly_event_t static *events = NULL;
ffly_event_t static *fresh_event;
ffly_event_t static **free_events = NULL;
ffly_event_t static **next_free = NULL;

ff_eventp ff_event_alloc(ff_err_t *__err) {
	*__err = FFLY_SUCCESS;
	if (!events) {
		if ((events = (ffly_event_t*)__ffly_mem_alloc(FAST_EVENTS*sizeof(ffly_event_t))) == NULL) {
			*__err = FFLY_FAILURE;
			return NULL;
		}
		fresh_event = events;
	}
    
	ffly_fprintf(ffly_log, "alloced event.\n");
	if (next_free>free_events)
		return *(--next_free);
	if (fresh_event >= events+FAST_EVENTS) {
		ffly_printf("fast full, pure alloc.\n");
		return (ffly_event_t*)__ffly_mem_alloc(sizeof(ffly_event_t));
	}
	return fresh_event++;
}

ff_err_t ff_event_free(ff_eventp __event) {
	if (!__event) return FFLY_SUCCESS;
	if (!free_events) {
		if ((free_events = (ffly_event_t**)__ffly_mem_alloc(FAST_EVENTS*sizeof(ffly_event_t*))) == NULL) {
			return FFLY_FAILURE;
		}
		next_free = free_events;
	}

	ffly_fprintf(ffly_log, "freed event.\n");
	if (__event<events && __event >= events+FAST_EVENTS) {
		__ffly_mem_free(__event);
	} else {
		if (__event == fresh_event-1)
			fresh_event--;
		else {
			if (next_free >= free_events+FAST_EVENTS) {
				ffly_fprintf(ffly_err, "error.\n");
			}
			*(next_free++) = __event;
		}
	}
	return FFLY_SUCCESS;
}

void ff_event_cleanup(void) {
	if (events != NULL)
		__ffly_mem_free(events);
	if (free_events != NULL)
		__ffly_mem_free(free_events);
}
