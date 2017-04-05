# ifndef __colour__t__hpp
# define __colour__t__hpp
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace graphics {

typedef struct {
	boost::uint8_t r, g, b, a;
} colour_t;

typedef struct {
	boost::uint8_t r, g, b, a;
} rgba_colour_t;

typedef struct {
	boost::uint8_t r, g, b;
} rgb_colour_t;

static colour_t __colour__(boost::uint8_t __r, boost::uint8_t __g, boost::uint8_t __b, boost::uint8_t __a) {
	colour_t _colour = {
		.r = __r,
		.g = __g ,
		.b = __b,
		.a = __a
	};
	return _colour;
}

}
}
}

# endif /*__colour__t__hpp*/
