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

	types::coords_t<boost::int16_t> get_wd_coords() {
# if defined(USING_XCB)
		return this-> wd_handler.wd_coords;
# endif

# if defined(USING_X11)

	types::coords_t<boost::int16_t> wd_coords = {
		.xaxis = (boost::int16_t)this-> wd_handler.wd_coords.xaxis,
		.yaxis = (boost::int16_t)this-> wd_handler.wd_coords.yaxis
	};

	return wd_coords;
# endif
	}

	types::mouse_coords_t<boost::int16_t, boost::uint16_t> get_mouse_coords() {
# if defined(USING_XCB)
		return this-> wd_handler.mouse_coords;
# endif

# if defined(USING_X11)
		types::coords_t<boost::uint16_t> wd_coords = {
			.xaxis = (boost::uint16_t)this-> wd_handler.mouse_coords.wd.xaxis,
			.yaxis = (boost::uint16_t)this-> wd_handler.mouse_coords.wd.yaxis
		};

		types::coords_t<boost::int16_t> root_coords = {
			.xaxis = (boost::int16_t)this-> wd_handler.mouse_coords.root.xaxis,
			.yaxis = (boost::int16_t)this-> wd_handler.mouse_coords.root.yaxis
		};

		types::mouse_coords_t<boost::int16_t, boost::uint16_t> mouse_coords;
		mouse_coords.wd = wd_coords;
		mouse_coords.root = root_coords;

		return mouse_coords;
# endif
	}

	boost::int8_t de_init() {

		if (this-> wd_handler.de_init() != FFLY_SUCCESS) {
			fprintf(stderr, "window: failed to 'de init' window handler.\n");
			return FFLY_FAILURE;
		}
		return FFLY_SUCCESS;
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
