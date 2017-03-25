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

}
}
}

# endif /*__coords__t__hpp*/
