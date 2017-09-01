# ifndef __gpu__dr__pm__mem__hpp
# define __gpu__dr__pm__mem__hpp
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace graphics {
typedef struct {
	uint_t *xfs = nullptr, *yfs = nullptr, *pb_xlen = nullptr, *pm_xlen = nullptr, *pm_xfs;
	types::pixmap_t pixbuff = nullptr, pixmap = nullptr;
	u16_t *angle = nullptr;
	bool inited = false;
	uint_t _pb_size{}, _pm_size{}, _xfs{}, _yfs{}, _pm_xfs{}, _pb_xlen{}, _pm_xlen{};
	u16_t _angle{};
} gpu_dr_pm_mem;
}
}
}

# endif /*__gpu__dr__pm__mem__hpp*/
