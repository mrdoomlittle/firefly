# ifndef __window__hpp
# define __window__hpp
# if defined(USING_X11)
# include "x11_window.hpp"
# elif defined(USING_XCB)
# include "xcb_window.hpp"
# endif
# include <cstdio>
# include <eint_t.hpp>
# include <boost/cstdint.hpp>
# include "../system/errno.h"
# include "../types/pixmap_t.h"
# include "draw_pixmap.hpp"
# define WD_TO_CLOSE 16
# define WD_CLOSED 1
# define WD_OPEN 2
# define WD_WAITING 4
# define WD_DONE_DRAW 8
namespace mdl {
namespace firefly {
namespace graphics {
class window {
	public:
	boost::int8_t init(uint_t __wd_xlen, uint_t __wd_ylen, char const *__frame_title);
	boost::int8_t begin();

	types::pixmap_t get_pixbuff();

	boost::int8_t set_pixbuff(types::pixmap_t __pixmap, uint_t __xlen = 0, uint_t __ylen = 0);

	private:
	bool init_report = FFLY_FAILURE;
	uint_t wd_xlen = 0, wd_ylen = 0;
	char const *frame_title = nullptr;
	public:
# if defined(USING_X11)
	x11_window wd_handler;
# elif defined(USING_XCB)
	xcb_window wd_handler;
# endif
};
}
}
}

# endif /*__window__hpp*/
