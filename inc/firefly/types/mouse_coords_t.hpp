# ifndef __mouse__coords__t__hpp
# define __mouse__coords__t__hpp
# include "coords_t.hpp"
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace types {
template<typename _T1 = boost::int16_t, typename _T2 = boost::uint16_t>
struct mouse_coords_t {
	coords_t<_T1> root;
	coords_t <_T2> wd;
};
}
}
}

# endif /*__mouse__coords__t__hpp*/
