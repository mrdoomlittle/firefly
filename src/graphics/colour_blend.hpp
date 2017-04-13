# ifndef __colour__blend__hpp
# define __colour__blend__hpp
# include "../types/colour_t.hpp"
# include "../types/byte_t.h"
namespace mdl {
namespace firefly {
namespace graphics {
__inline__ types::byte_t colour_blend(types::byte_t __a, types::byte_t __b) {
	return (types::byte_t)(__a + __b / 2);
}
__inline__ rgb_colour_t colour_blend(rgb_colour_t& __a, rgb_colour_t& __b) {
	rgb_colour_t nw_colour = {
		.r = colour_blend(__a.r, __b.r),
		.g = colour_blend(__a.g, __b.g),
		.b = colour_blend(__a.b, __b.b)
	};
	return nw_colour;
}
}
}
}

# endif /*__colour__blend__hpp*/
