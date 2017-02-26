# ifndef __scale__pixmap__hpp
# define __scale__pixmap__hpp
# include <eint_t.hpp>
# include <boost/cstdint.hpp>
# include <cstdio>
namespace mdl {
namespace firefly {
namespace graphics {
boost::int8_t scale_pixmap(boost::uint8_t *&__pixmap, uint_t& __pixmap_xlen, uint_t& __pixmap_ylen, uint_t __scale);
}
}
}

# endif /*__scale__pixmap__hpp*/
