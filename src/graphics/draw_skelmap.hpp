# ifndef __draw__skellmap__hpp
# define __draw__skellmap__hpp
# include <mdlint.h>
# include "../types/colour_t.hpp"
# include "../types/skelmap_info_t.hpp"
# include <cstdio>
namespace mdl {
namespace firefly {
namespace graphics {
i8_t draw_skelmap(u8_t *__skelmap, u8_t *__pixbuff, uint_t __xaxis, uint_t __yaxis, types::skelmap_info_t skelmap_info, uint_t __pixbuff_xlen, uint_t __pixbuff_ylen, uint_t __scale, colour_t __colour);
}
}
}

# endif /*__draw__skellmap__hpp*/
