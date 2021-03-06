# ifndef __draw__bitmap__hpp
# define __draw__bitmap__hpp
# include "../types/bitmap_t.hpp"
# include "../types/pixmap_t.h"
# include "../types/coords_t.h"
# include "../types/colour_t.h"
# include <mdlint.h>
# include <cstdio>
# include "../system/errno.h"
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace graphics {
boost::int8_t draw_bitmap(types::bitmap_t __bitmap, uint_t __bm_xlen, uint_t __bm_ylen, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::_2d_coords_t __coords, types::colour_t __colour);
}
}
}


# endif /*__draw__bitmap__hpp*/
