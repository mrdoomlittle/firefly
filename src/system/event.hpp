# ifndef __system__event__hpp
# define __system__event__hpp
# include <mdlint.h>
# include "../types/err_t.h"
# include "../types/event_t.h"
# include "../types/bool_t.h"
/*
# include <queue>
# include "errno.h"
# include "../types/event_t.hpp"
# include "../types/event_desc_t.hpp"
# include "../types/err_t.h"
# include "../data/pair.hpp"
# include "../types/btn_event_t.hpp"
*/
namespace mdl {
namespace firefly {
namespace system {
types::err_t event_add(types::event_t __event);
types::err_t next_event(types::event_t& __event);
types::bool_t pending_event();
/*
types::err_t event_add(types::event_desc_t __event_desc, void *__event_data);
void* nxt_event(types::event_desc_t& __event_desc);
void event_pop();

uint_t max_queue_size();
void max_queue_size(uint_t __queue_size);

void *event_data(void *);

std::queue<data::pair<types::event_desc_t, void*>> extern event_queue;
*/
}
}
}

# endif /*__system__event__hpp*/
