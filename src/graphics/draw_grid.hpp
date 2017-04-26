# ifndef __draw__grid__hpp
# define __draw__grid__hpp
# include <eint_t.hpp>
# include <cstdio>
# include "../types/err_t.h"
# include "../system/errno.h"
# include "../types/pixmap_t.h"
# include "../types/colour_t.hpp"
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t cpu_draw_grid(types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, uint_t __xsplit, uint_t __ysplit, uint_t __horz_thickness, uint_t __vert_thickness, colour_t __line_colour);
types::err_t static draw_grid(types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, uint_t __xsplit, uint_t __ysplit, uint_t __horz_thickness, uint_t __vert_thickness, colour_t __line_colour) {
	cpu_draw_grid(__pixmap, __pm_xlen, __pm_ylen, __xsplit, __ysplit, __horz_thickness, __vert_thickness, __line_colour);
}
}
}
}

# endif /*__draw__grid__hpp*/
