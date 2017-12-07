# ifndef __draw__pixel__hpp
# define __draw__pixel__hpp
# include "../types/colour_t.hpp"
# include <mdlint.h>
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace graphics {
void draw_pixel(uint_t __xaxis, uint_t __yaxis, colour_t __colour, boost::uint8_t *__pixbuff, uint_t __pb_xlen) {
	uint_t point = (__xaxis + (__yaxis * __pb_xlen)) * 4;
	__pixbuff[point] = __colour.r;
	__pixbuff[point + 1] = __colour.g;
	__pixbuff[point + 2] = __colour.b;
	__pixbuff[point + 3] = __colour.a;
}
}
}
}

# endif /*__draw__pixel__hpp*/
