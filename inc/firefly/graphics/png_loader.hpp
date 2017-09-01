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
# include "../types/dsize_t.hpp"
# include "../memory/mem_alloc.h"
# include "../types/err_t.h"
# include "../system/io.h"
# include <errno.h>
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t load_png_file(char const *__filedir, char const *__filename, types::pixmap_t& __pixmap, uint_t *__pm_size);
types::err_t load_png_file(char const *__filedir, char const *__filename, types::pixmap_t& __pixmap, types::_2d_dsize_t<>& __pm_size);
}
}
}
# endif /*__png__loader__hpp*/
