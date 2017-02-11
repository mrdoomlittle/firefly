# ifndef __png__loader__hpp
# define __png__loader__hpp
# include <fstream>
# include <png.h>
# include <string.h>
# include <boost/cstdint.hpp>
namespace mdl {
boost::uint8_t load_png_file(char const *__filename, boost::uint8_t *&__pixmap, int unsigned *__pixmap_size);
}
# endif /*__png__loader__hpp*/
