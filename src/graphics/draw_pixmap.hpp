# ifndef __graphics__draw__pixmap__hpp
# define __graphics__draw__pixmap__hpp
# include <eint_t.hpp>
# include <cstdio>
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace graphics {
boost::int8_t draw_pixmap(uint_t __xoffset, uint_t __yoffset, boost::uint8_t *__pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, boost::uint8_t *__pixmap, uint_t __pm_xlen, uint_t __pm_ylen);
}
}
}

# endif /*__graphics__draw__pixmap__hpp*/
