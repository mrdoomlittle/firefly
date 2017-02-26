# ifndef __fill_pixmap__hpp
# define __fill_pixmap__hpp
# include "../types/colour_t.hpp"
# include <eint_t.hpp>
# include <boost/cstdint.hpp>
# include <cstdio>
namespace mdl {
namespace firefly {
namespace graphics {
boost::int8_t fill_pixmap(boost::uint8_t *__pixmap, uint_t __xlen, uint_t __ylen, colour_t __colour);
}
}
}

# endif /*__fill_pixmap__hpp*/
