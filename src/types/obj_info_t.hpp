# ifndef __obj__info__t__hpp
# define __obj__info__t__hpp
# include <eint_t.hpp>
# include "colour_t.hpp"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	bool gravity_enabled = false;
	uint_t gravity_speed = 100; // pixels per second
	bool bound_enabled = false;
	uint_t xaxis_bound[2], yaxis_bound[2], zaxis_bound[2];
	uint_t xaxis, yaxis, zaxis;
	uint_t xaxis_len, yaxis_len, zaxis_len;
	graphics::colour_t def_colour;
	bool skip_handle = false;
} obj_info_t;
}
}
}

# endif /*__obj__info__t__hpp*/
