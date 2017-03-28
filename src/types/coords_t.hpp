# ifndef __coords__t__hpp
# define __coords__t__hpp
# include <eint_t.hpp>

namespace mdl {
namespace firefly {
namespace types {
template <typename _T = uint_t>
struct coords_t {
	_T xaxis, yaxis, zaxis;
};
// ncvv seems to not like this
# ifndef __NVCC__
template <typename _T = uint_t>
coords_t<_T> coords(_T __xaxis = 0, _T __yaxis = 0, _T __zaxis = 0) {
	coords_t<_T> _coords = {
		.xaxis = __xaxis,
		.yaxis = __yaxis,
		.zaxis = __zaxis
	};
	return _coords;
}
# endif
}
}
}

# endif /*__coords__t__hpp*/
