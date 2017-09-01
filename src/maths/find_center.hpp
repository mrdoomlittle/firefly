# ifndef __find__center__hpp
# define __find__center__hpp
# include <math.h>
# include <mdlint.h>
namespace mdl {
namespace firefly {
namespace maths {
template<typename _T = uint_t>
_T __inline__ find_center(_T __no) {
	return (_T)round((double)__no/2.0);
}
}
}
}


# endif /*__find__center__hpp*/
