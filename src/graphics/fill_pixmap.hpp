# ifndef __fill_pixmap__hpp
# define __fill_pixmap__hpp
# include "../types/colour_t.hpp"
# include <eint_t.hpp>
# include <boost/cstdint.hpp>
# include <cstdio>
# include "../system/errno.h"
# include "../types/pixmap_t.h"
# include "../types/err_t.h"
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t fill_pixmap(types::pixmap_t __pixmap, uint_t __xaxis_len, uint_t __yaxis_len, colour_t __colour);
}
}
}

# endif /*__fill_pixmap__hpp*/
