# ifndef __draw__outline__hpp
# define __draw__outline__hpp
# define SPE_RECT 0
# include <boost/cstdint.hpp>
# include "../types/coords_t.hpp"
# include "../types/pixmap_t.h"
# include "../types/dsize_t.hpp"
# include "../types/colour_t.hpp"
# include "draw_rect.hpp"
# include "../types/err_t.h"
namespace mdl {
namespace firefly {
namespace graphics {
# if defined(USING_CUDA) || defined(__USING_CUDA)
/*
	this will be replaced later on.
*/
types::err_t static draw_outline(types::pixmap_t __pixbuff, types::coords_t<> __bcoords, types::coords_t<> __ecoords, uint_t __ol_width, types::dsize_t __pb_size, graphics::colour_t __colour) {
	if (draw_rect(__pixbuff, __bcoords.xaxis - __ol_width, __bcoords.yaxis - __ol_width, __ecoords.xaxis + __ol_width, __bcoords.yaxis, __colour, __pb_size.xaxis_len, __pb_size.yaxis_len) != FFLY_SUCCESS) return FFLY_FAILURE;
	if (draw_rect(__pixbuff, __bcoords.xaxis - __ol_width, __bcoords.yaxis, __bcoords.xaxis, __ecoords.yaxis, __colour, __pb_size.xaxis_len, __pb_size.yaxis_len) != FFLY_SUCCESS) return FFLY_FAILURE;
	if (draw_rect(__pixbuff, __ecoords.xaxis, __bcoords.yaxis, __ecoords.xaxis + __ol_width, __ecoords.yaxis, __colour, __pb_size.xaxis_len, __pb_size.yaxis_len) != FFLY_SUCCESS) return FFLY_FAILURE;
	if (draw_rect(__pixbuff, __bcoords.xaxis - __ol_width, __ecoords.yaxis, __ecoords.xaxis + __ol_width, __ecoords.yaxis + __ol_width, __colour, __pb_size.xaxis_len, __pb_size.yaxis_len) != FFLY_SUCCESS) return FFLY_FAILURE;
	return FFLY_SUCCESS;
}
# endif
}
}
}


# endif /*__draw__outline__hpp*/
