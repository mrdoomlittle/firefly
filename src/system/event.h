# ifndef __ffly__event__h
# define __ffly__event__h
# include "../ffint.h"
# include "../types.h"
# include "../types/event_t.h"
# include "queue.h"
# define ffly_build_event(__field, __kind, __data, __p) \
    *(__p) = (ffly_event_t){.field=__field, .kind=__kind, .data=(void*)__data}
# ifdef __cplusplus
extern "C" {
# endif
ffly_event_t* ffly_event_dup(ffly_event_t*);
extern struct ffly_queue ffly_event_queue;
ff_err_t ffly_event_push(ffly_event_t*);
ff_err_t ffly_event_pop(ffly_event_t**);
ff_err_t ffly_event_peek(ffly_event_t**);
ff_bool_t ffly_pending_event();
ffly_event_t *ffly_alloc_event(ff_err_t*);
ff_err_t ffly_free_event(ffly_event_t*);
/*
    event intercept routine - READ ONLY.
    - wont effect queue.
*/
ff_err_t ffly_add_eir(ff_err_t(*)(ffly_event_t*, void*), void*);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace system {
static types::err_t(*event_push)(types::event_t*) = &ffly_event_push;
static types::err_t(*event_pop)(types::event_t**) = &ffly_event_pop;
static types::err_t(*event_peek)(types::event_t**) = &ffly_event_peek;
static types::bool_t(*pending_event)() = &ffly_pending_event;
}
}
}
# endif
# endif /*__ffly__event__h*/
