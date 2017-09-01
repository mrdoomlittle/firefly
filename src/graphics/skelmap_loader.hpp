# ifndef __skelmap__loader__hpp
# define __skelmap__loader__hpp
# include "../types/skelmap_info_t.hpp"
# include <string.h>
# include <mdlint.h>
# include <fstream>
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace graphics {
boost::uint8_t* load_skemap_file(types::skelmap_info_t& skelmap_info, char const *__file);
}
}
}

# endif /*__skelmap__loader__hpp*/
