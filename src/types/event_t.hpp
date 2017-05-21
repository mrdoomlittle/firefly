# ifndef __event__t__hpp
# define __event__t__hpp
# include "event_disc_t.hpp"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	event_disc_t event_disc;
	void *event_data;
} event_t;
}
}
}

# endif /*__event__t__hpp*/
