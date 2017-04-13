# ifndef __colour__t__hpp
# define __colour__t__hpp
# include "../types/byte_t.h"
namespace mdl {
namespace firefly {
namespace graphics {

typedef struct {
	types::byte_t r, g, b, a;
} colour_t;

typedef struct {
	types::byte_t r, g, b, a;
} rgba_colour_t;

typedef struct {
	types::byte_t r, g, b;
} rgb_colour_t;

static colour_t __colour__(types::byte_t __r, types::byte_t __g, types::byte_t __b, types::byte_t __a) {
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
