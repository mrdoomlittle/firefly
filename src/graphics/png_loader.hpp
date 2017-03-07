# ifndef __png__loader__hpp
# define __png__loader__hpp
# include <fstream>
# include <png.h>
# include <string.h>
# include <boost/cstdint.hpp>
# include <eint_t.hpp>
# include "../system/errno.h"
# include <strcmb.hpp>
# include "../types/pixmap_t.h"
namespace mdl {
namespace firefly {
namespace graphics {
boost::int8_t load_png_file(char const *__filedir, char const *__filename, types::pixmap_t&__pixmap, uint_t *__pm_size);
}
}
}
# endif /*__png__loader__hpp*/
