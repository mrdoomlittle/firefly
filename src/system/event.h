# ifndef __ffly__event__h
# define __ffly__event__h
# include "../types/event_t.h"
# include "../types/bool_t.h"
# include "../types/err_t.h"
# include "queue.h"
# ifdef __cplusplus
extern "C" {
# endif
extern struct ffly_queue ffly_event_queue;
ffly_err_t ffly_event_push(ffly_event_t*);
ffly_err_t ffly_event_pop(ffly_event_t**);
ffly_bool_t ffly_pending_event();
ffly_event_t *ffly_alloc_event();
void ffly_free_event(ffly_event_t*);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace system {
static types::err_t(*event_push)(types::event_t*) = &ffly_event_push;
static types::err_t(*event_pop)(types::event_t**) = &ffly_event_pop;
static types::bool_t(*pending_event)() = &ffly_pending_event;
}
}
}
# endif
# endif /*__ffly__event__h*/
