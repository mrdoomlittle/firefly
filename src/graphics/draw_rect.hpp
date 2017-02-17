# ifndef __graphics__draw__rect__hpp
# define __graphics__draw__rect__hpp
# include <boost/cstdint.hpp>
# include <eint_t.hpp>
# include <emu2d.hpp>
# include "../types/colour_t.hpp"
namespace mdl {
namespace firefly {
namespace graphics {
void draw_rect(boost::uint8_t *__pixbuff, uint_t __b_xcoord, uint_t __b_ycoord, uint_t __e_xcoord, uint_t __e_ycoord, colour_t __colour, uint_t __pd_xlen) {
	for (std::size_t yaxis = __b_ycoord; yaxis != __e_ycoord; yaxis ++) {
		for (std::size_t xaxis = __b_xcoord; xaxis != __e_xcoord; xaxis ++) {
			uint_t pixpos = emu2d(xaxis, yaxis, __pd_xlen, 0, 4);
			__pixbuff[pixpos] = __colour.r;
			__pixbuff[pixpos + 1] = __colour.g;
			__pixbuff[pixpos + 2] = __colour.b;
			__pixbuff[pixpos + 3] = __colour.a;
		}
	}
}


}
}
}

# endif /*__graphics__draw__rect__hpp*/
