# ifndef __scale__pixmap__hpp
# define __scale__pixmap__hpp
# include "../types/pixmap_t.h"
# include "../types/err_t.h"
# include <mdlint.h>
# include "../memory/alloc_pixmap.hpp"
# include "../memory/mem_free.h"
# include "../system/errno.h"
# include <string.h>
# include <cstdio>
# include <math.h>
# include "../graphics/colour_blend.hpp"
# include "../types/byte_t.h"
# define CUBIC_SCALE 0
// i was trying but failed to do this
# define LINEAR_SCALE 1
namespace mdl {
namespace firefly {
namespace data {
types::err_t gpu_scale_pixmap();
types::err_t cpu_scale_pixmap(types::pixmap_t& __pixmap, uint_t& __xaxis_len, uint_t& __yaxis_len, int_t __xscale, int_t __yscale, boost::int8_t __type);

types::err_t static scale_pixmap(types::pixmap_t& __pixmap, uint_t& __xaxis_len, uint_t& __yaxis_len, int_t __xscale, int_t __yscale, boost::int8_t __type) {
	cpu_scale_pixmap(__pixmap, __xaxis_len, __yaxis_len, __xscale, __yscale, __type);
}
}
}
}

# endif /*__scale__pixmap__hpp*/
