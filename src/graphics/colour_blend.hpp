# ifndef __colour__blend__hpp
# define __colour__blend__hpp
# include <boost/cstdint.hpp>
# include "../types/colour_t.hpp"
namespace mdl {
namespace firefly {
namespace graphics {
__inline__ colour_t colour_blend(colour_t& __a, colour_t& __b) {
	colour_t n_colour = {
		.r = (boost::uint8_t)__a.r + __b.r / 2,
		.g = (boost::uint8_t)__a.g + __b.g / 2,
		.b = (boost::uint8_t)__a.b + __b.b / 2,
		.a = (boost::uint8_t)__a.a + __b.a / 2
	};
	return n_colour;
}
}
}
}

# endif /*__colour__blend__hpp*/
