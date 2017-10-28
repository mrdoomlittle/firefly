# ifndef __gpu__fl__pm__mem__hpp
# define __gpu__fl__pm__mem__hpp
# include "../types/pixelmap_t.h"
# include "../types/bool_t.h"
# include "../types/colour_t.h"
namespace mdl {
namespace firefly {
namespace graphics {
struct gpu_fl_pm_mem {
	types::pixelmap_t pixelmap;
	mdl_uint_t pixelmap_size;
	types::colour_t *colour, _colour;
	types::bool_t inited;
};
}
}
}
# endif /*__gpu__fl__pm__mem__hpp*/
