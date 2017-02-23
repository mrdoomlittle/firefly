# ifndef __layering__hpp
# define __layering__hpp
# include "../layer.hpp"
# include <stdlib.h>
# include <cstdio>
# include <boost/cstdint.hpp>
# include "../types/colour_t.hpp"
# include "../graphics/fill_pixmap.hpp"
namespace mdl {
namespace firefly {
namespace tests {
void layering(uint_t __max_xlen, uint_t __min_xlen, uint_t __max_ylen, uint_t __min_ylen, layer *__layer_ptr, uint_t __amount, uint_t __spacing, boost::uint8_t __alpha = 111);
}
}
}

# endif /*__layering__hpp*/
