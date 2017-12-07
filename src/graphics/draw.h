# ifndef __ffly__draw__h
# define __ffly__draw__h
# include "../types/pixelmap_t.h"
# include "../types/off_t.h"
# include "../types/err_t.h"
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t draw_pixelmap(types::off_t __xfs, types::off_t __yfs,
	types::pixelmap_t __pixelbuff, uint_t __pb_xal, uint_t __pb_yal,
	types::pixelmap_t __pixelmap, uint_t __pm_xal, uint_t __pm_yal, uint_t __rpm_xal);
}
}
}
# endif
# endif /*__ffly__draw__h*/
