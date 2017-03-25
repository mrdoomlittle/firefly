# ifndef __rotate__point__hpp
# define __rotate__point__hpp
# include <eint_t.hpp>
# include <math.h>
# include "pi.h"
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace maths {

// NOTE: xaxis and yaxis is for the thing your rotating not where it is in the world
// e.g. if i was to rotate a rect 8x6 i would start from 0 to etc.
// other thing the offset should we where it is in the world.
void rotate_point(uint_t& __xaxis, uint_t& __yaxis, boost::uint16_t __angle, uint_t __xoffset, uint_t __yoffset, boost::int8_t __dir);
}
}
}

# endif /*__rotate__point__hpp*/
