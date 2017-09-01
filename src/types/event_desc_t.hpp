# ifndef __event__desc__t__hpp
# define __event__desc__t__hpp
# include "event_id_t.hpp"
# include <mdlint.h>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	event_id_t event_id;
	uint_t event_type;
} event_desc_t;
}
}
}

# endif /*__event__desc__t__hpp*/
