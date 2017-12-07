# ifndef __draw__line__hpp
# define __draw__line__hpp
# include <boost/cstdint.hpp>
# include <mdlint.h>
# include <math.h>
# include <emu2d.hpp>
# include "../types/colour_t.hpp"
namespace mdl {
namespace firefly {
namespace graphics {
void draw_line(boost::uint8_t *__pixbuff, uint_t __b_xcoord, uint_t __b_ycoord, uint_t __e_xcoord, uint_t __e_ycoord, colour_t __colour, uint_t __pd_xlen) {
	uint_t deltax = 0, deltay = 0;
	if (__b_xcoord < __e_xcoord)
		deltax = __e_xcoord - __b_xcoord;
	else
		deltax = __b_xcoord - __e_xcoord;

	if (__b_ycoord < __e_ycoord)
		deltay = __e_ycoord - __b_ycoord;
	else
		deltay = __b_ycoord - __e_ycoord;

	uint_t deltaerr = abs(deltay / deltax);
	uint_t error = deltaerr - 0.5;

	uint_t yaxis = __b_ycoord;
	for (std::size_t xaxis = __b_xcoord; xaxis != __b_xcoord + __e_xcoord; xaxis ++) {
		uint_t pixpos = emu2d(xaxis, yaxis, __pd_xlen, 0, 4);
		__pixbuff[pixpos] = __colour.r;
		__pixbuff[pixpos + 1] = __colour.g;
		__pixbuff[pixpos + 2] = __colour.b;
		__pixbuff[pixpos + 3] = __colour.a;

		error = error + deltaerr;
		if (error > 0.5) {
			yaxis = yaxis + 1;
			error = error - 1.0;
		}
	}
}
}
}
}

# endif /*__draw__line__hpp*/
