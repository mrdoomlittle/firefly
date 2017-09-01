# ifndef __core__portal__h
# define __core__portal__h
# include "graphics/pointer_coords.h"
# include "types/coords_t.hpp"
namespace mdl {
namespace firefly {
struct _core_portal {
	graphics::_pointer_coords<i32_t, i32_t> *pointer_coords;
	types::_2d_coords_t<i32_t> *wd_coords;
	u16_t *wd_xa_len, *wd_ya_len;
};

struct _core_portal extern core_portal;
}
}

# endif /*__core__portal__h*/
