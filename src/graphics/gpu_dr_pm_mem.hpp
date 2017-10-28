# ifndef __gpu__dr__pm__mem__hpp
# define __gpu__dr__pm__mem__hpp
# include <mdlint.h>
# include "../types/pixelmap_t.h"
namespace mdl {
namespace firefly {
namespace graphics {
typedef struct {
	uint_t *xfs = nullptr, *yfs = nullptr, *pb_xa_len = nullptr, *pm_xa_len = nullptr, *pm_xfs;
	types::pixelmap_t pixelbuff = nullptr, pixelmap = nullptr;
	u16_t *angle = nullptr;
	bool inited = false;
	uint_t _pb_size{}, _pm_size{}, _xfs{}, _yfs{}, _pm_xfs{}, _pb_xa_len{}, _pm_xa_len{};
	u16_t _angle{};
} gpu_dr_pm_mem;
}
}
}

# endif /*__gpu__dr__pm__mem__hpp*/
