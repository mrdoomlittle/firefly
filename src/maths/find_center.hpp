# ifndef __find__center__hpp
# define __find__center__hpp
# include <math.h>
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace maths {
template<typename _T = uint_t>
__inline__ _T find_center(_T __no) {
	return (_T)round(__no / 2);
}
}
}
}


# endif /*__find__center__hpp*/
