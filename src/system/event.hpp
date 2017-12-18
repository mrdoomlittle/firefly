# ifndef __system__event__hpp
# define __system__event__hpp
# include <mdlint.h>
# include "../types/err_t.h"
# include "../types/event_t.h"
# include "../types/bool_t.h"
namespace mdl {
namespace firefly {
namespace system {
types::err_t event_push(types::event_t *__event);
types::err_t next_event(types::event_t*& __event);
types::bool_t pending_event();
}
}
}

# endif /*__system__event__hpp*/
