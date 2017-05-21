# ifndef __system__event__hpp
# define __system__event__hpp
# include <eint_t.hpp>
# include <queue>
# include "errno.h"
# include "../types/event_t.hpp"
# include "../types/event_disc_t.hpp"
# include "../types/err_t.h"
# include "../data/pair.hpp"
# include "../types/btn_event_t.hpp"
namespace mdl {
namespace firefly {
namespace system {
namespace event {

types::err_t event_add(types::event_disc_t __event_disc, void *__event_data);
void* nxt_event(types::event_disc_t& __event_disc);
void event_pop();

uint_t max_queue_size();
void max_queue_size(uint_t __queue_size);

void *event_data(void *);

std::queue<data::pair<types::event_disc_t, void*>> extern event_queue;
}
}
}
}

# endif /*__system__event__hpp*/
