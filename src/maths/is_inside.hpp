# ifndef __is__inside__hpp
# define __is__inside__hpp
# include "../types/coords_t.hpp"
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace maths {
__inline__ bool is_inside(types::coords_t<> coords_a, types::coords_t<> coords_b, uint_t __xaxis_len, uint_t __yaxis_len, uint_t __zaxis_len) {
	if ((coords_a.xaxis >= coords_b.xaxis && coords_a.yaxis >= coords_b.yaxis && coords_a.zaxis >= coords_b.zaxis)
	&& (coords_a.xaxis <= coords_b.xaxis + __xaxis_len && coords_a.yaxis <= coords_b.yaxis  + __yaxis_len && coords_a.zaxis <= coords_b.zaxis + __zaxis_len)) return true;
	return false;
}
}
}
}


# endif /*__is__inside__hpp*/
