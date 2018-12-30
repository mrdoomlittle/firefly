# ifndef __ffly__pixelmap__hpp
# define __ffly__pixelmap__hpp
# include "../ffly_graphics.hpp"
# include "../types/err_t.h"
# include "../types/pixelmap_t.h"
# include "../types/colour_t.h"
# include "gpu_fl_pm_mem.hpp"

namespace mdl {
namespace firefly {
namespace graphics {
void gpu_fill_pixelmap(gpu_fl_pm_mem& __mem, types::pixelmap_t __pixelmap, mdl_uint_t __xa_len, mdl_uint_t __ya_len, types::colour_t __colour);
}
}
}
# endif /*__ffly__pixelmap__hpp*/
