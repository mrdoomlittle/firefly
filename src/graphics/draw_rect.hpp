# ifndef __graphics__draw__rect__hpp
# define __graphics__draw__rect__hpp
# include <boost/cstdint.hpp>
# include <eint_t.hpp>
# include <emu2d.hpp>
# include "../types/colour_t.hpp"
namespace mdl {
namespace firefly {
namespace graphics {
void draw_rect(boost::uint8_t *__pixbuff, uint_t __b_xcoord, uint_t __b_ycoord, uint_t __e_xcoord, uint_t __e_ycoord, colour_t __colour, uint_t __pd_xlen, uint_t __pd_ylen);
}
}
}

# endif /*__graphics__draw__rect__hpp*/
