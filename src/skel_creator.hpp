# ifndef __skel__creator__hpp
# define __skel__creator__hpp
# include "types/err_t.h"
# include "types/pixmap_t.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_init.h"
# include <mdlint.h>
# include "system/errno.h"
# include <errno.h>
# include <math.h>
# include "graphics/draw_rect.hpp"
# include "graphics/draw_pixmap.hpp"
# include "graphics/draw_grid.hpp"
# include "types/colour_t.h"
# include "memory/alloc_pixmap.h"
namespace mdl {
namespace firefly {
class skel_creator {
	public:
	types::err_t init(uint_t __pm_xlen, uint_t __pm_ylen, uint_t __xsplit, uint_t __ysplit);
	types::err_t de_init();
	types::err_t tick(u16_t __mx_axis, u16_t __my_axis, bool __mpress, int __button_code);
	types::err_t draw(types::_1d_pm_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen);
	private:
	u8_t *blk_st;
	uint_t xsplit, ysplit;
	uint_t bpm_xa_len, bpm_ya_len;
	types::_1d_pm_t bse_pm = nullptr;
};
}
}

# endif /*__skel__creator__hpp*/
