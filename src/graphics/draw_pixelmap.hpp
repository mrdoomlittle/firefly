# ifndef __ffly__draw__pixelmap__hpp
# define __ffly__draw__pixelmap__hpp
# include "gpu_dr_pm_mem.hpp"
# include "../types/err_t.h"
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t gpu_draw_pixelmap(gpu_dr_pm_mem& __gpu_dr_pm_mem, uint_t __xfs, uint_t __yfs, types::pixelmap_t __pixelbuff, uint_t __pb_xa_len, uint_t __pb_ya_len, uint_t __pm_xfs, types::pixelmap_t __pixelmap, uint_t __pm_xa_len, uint_t __pm_ya_len, uint_t __rpm_xa_len, u16_t __angle);

types::err_t static gpu_draw_pixelmap(uint_t __xfs, uint_t __yfs, types::pixelmap_t __pixelbuff, uint_t __pb_xa_len, uint_t __pb_ya_len, types::pixelmap_t __pixelmap, uint_t __pm_xa_len, uint_t __pm_ya_len) {
	gpu_dr_pm_mem static gpu_dr_pm_mem;
	return gpu_draw_pixelmap(gpu_dr_pm_mem, __xfs, __yfs, __pixelbuff, __pb_xa_len, __pb_ya_len, 0, __pixelmap, __pm_xa_len, __pm_ya_len, __pm_xa_len, 0);
}
}
}
}
# endif /*__ffly__draw__pixelmap__hpp*/
