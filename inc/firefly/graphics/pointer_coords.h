# ifndef __pointer__coords
# define __pointer__coords
# include "../types/coords_t.hpp"
namespace mdl {
namespace firefly {
namespace graphics {
template<typename _WD_T, typename _ROOT_T>
struct _pointer_coords {
	types::_2d_coords_t<_WD_T> wd;
	types::_2d_coords_t<_ROOT_T> root;
};
}
}
}
# endif /*__pointer__coords*/
