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
# include "fill_pixmap.hpp"
# include "../types/colour_t.hpp"
# include "../types/coords_t.hpp"
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
	boost::int8_t init(boost::uint16_t __wd_xaxis_len, boost::uint16_t __wd_yaxis_len, char const *__frame_title);
	boost::int8_t begin();

	types::pixmap_t get_pixbuff();

	boost::int8_t set_pixbuff(types::pixmap_t __pixmap, boost::uint16_t __xaxis_len = 0, boost::uint16_t __yaxis_len = 0);

	void clear_pixbuff() {
		colour_t colour = { 0, 0, 0, 0 };
		this-> clear_pixbuff(colour);
	}

	void clear_pixbuff(colour_t __colour) {
		fill_pixmap(this-> get_pixbuff(), this-> wd_xaxis_len, this-> wd_yaxis_len, __colour);
	}

	types::coords_t get_wd_coords() {
		types::coords_t wd_coords = {
			wd_handler.window_coords.xaxis,
			wd_handler.window_coords.yaxis,
		};
		return wd_coords;
	}

	types::coords_t get_mouse_coords() {
		types::coords_t mouse_coords = {
			wd_handler.mouse_coords.w_xaxis,
			wd_handler.mouse_coords.w_yaxis
		};
		return mouse_coords;
	}

	private:
	bool init_report = FFLY_FAILURE;
	boost::uint16_t wd_xaxis_len = 0, wd_yaxis_len = 0;
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
