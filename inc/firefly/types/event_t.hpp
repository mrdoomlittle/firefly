# ifndef __event__t__hpp
# define __event__t__hpp
# include "event_desc_t.hpp"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	event_desc_t event_desc;
	void *event_data;
} event_t;
}
}
}

# endif /*__event__t__hpp*/
