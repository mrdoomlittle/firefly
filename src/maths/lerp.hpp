# ifndef __lerp__hpp
# define __lerp__hpp
# include <mdlint.h>
namespace mdl {
namespace firefly {
namespace maths {
__inline__ lerp(uint_t __norm, uint_t __min, uint_t __max) {
	return __max-__min*__norm+__min;
}
}
}
}

# endif /*__lerp__hpp*/
