# ifndef __uni__par__hpp
# define __uni__par__hpp
# include "colour_t.hpp"
namespace mdl {
namespace firefly {
namespace types {
typedef struct uni_par {
	uni_par() : colour(nullptr) {}

	graphics::colour_t *colour;
} uni_par_t;
}
}
}

# endif /*__uni__par__hpp*/
