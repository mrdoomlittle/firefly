# ifndef __event__disc__t__hpp
# define __event__disc__t__hpp
# include "event_id_t.hpp"
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	event_id_t event_id;
	uint_t event_type;
} event_disc_t;
}
}
}

# endif /*__event__disc__t__hpp*/
