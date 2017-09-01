# ifndef __window__hpp
# define __window__hpp
# if defined(USING_X11)
# include "x11_window.hpp"
# elif defined(USING_XCB)
# include "xcb_window.hpp"
# endif
# include <cstdio>
# include <mdlint.h>
# include <boost/cstdint.hpp>
# include "../system/errno.h"
# include "../types/pixmap_t.h"
# include "draw_pixmap.hpp"
# include "fill_pixmap.hpp"
# include "../types/colour_t.hpp"
# include "../types/coords_t.hpp"
# include "../types/err_t.h"
# include "../types/event_desc_t.hpp"
//# include <queue>
# include "../system/mq_mask.hpp"
# include "../data/pair.hpp"
# define WD_TO_CLOSE 16
# define WD_CLOSED 1
# define WD_OPEN 2
# define WD_WAITING 4
# define WD_DONE_DRAW 8
namespace mdl {
namespace firefly {
namespace graphics {
class window {
# if defined(USING_X11)
	friend class x11_window;
# elif defined(USING_XCB)
	friend class xcb_window;
# endif
	public:
	types::err_t init(u16_t __wd_xaxis_len, u16_t __wd_yaxis_len, char const *__frame_title);
	types::err_t begin();

	types::pixmap_t get_pixbuff();

	bool __inline__ is_key_press() {return this->wd_handle.key_press;}
	bool __inline__& key_press() {return this->wd_handle.key_press;}

	int __inline__ get_key_code() {return this->wd_handle.key_code;}
	int __inline__& key_code() {return this->wd_handle.key_code;}

	bool __inline__ is_button_press() {return this->wd_handle.button_press;}
	bool __inline__& button_press() {return this->wd_handle.button_press;}

	int __inline__ get_button_code() {return this->wd_handle.button_code;}
	int __inline__& button_code() {return this->wd_handle.button_code;}

	types::err_t set_pixbuff(types::pixmap_t __pixmap, u16_t __xaxis_len = 0, u16_t __yaxis_len = 0);

	void clear_pixbuff() {
		colour_t colour = { 0, 0, 0, 0 };
		this->clear_pixbuff(colour);
	}

	void clear_pixbuff(colour_t __colour) {
		fill_pixmap(this->get_pixbuff(), this->wd_xaxis_len, this->wd_yaxis_len, __colour);
	}

	types::coords_t<i16_t> get_wd_coords() {
# if defined(USING_XCB)
		return this->wd_coords;
# endif

# if defined(USING_X11)
	types::coords_t<i16_t> wd_coords = {
		.xaxis = (i16_t)this->wd_coords.xaxis,
		.yaxis = (i16_t)this->wd_coords.yaxis
	};

	return wd_coords;
# endif
	}

	types::mouse_coords_t<i16_t, u16_t> get_mouse_coords() {
# if defined(USING_XCB)
		return this->pointer_coords;
# endif

# if defined(USING_X11)
		types::coords_t<u16_t> wd_coords = {
			.xaxis = (u16_t)this->pointer_coords.wd.xaxis,
			.yaxis = (u16_t)this->pointer_coords.wd.yaxis
		};

		types::coords_t<i16_t> root_coords = {
			.xaxis = (i16_t)this->pointer_coords.root.xaxis,
			.yaxis = (i16_t)this->pointer_coords.root.yaxis
		};

		types::mouse_coords_t<i16_t, u16_t> mouse_coords;
		mouse_coords.wd = wd_coords;
		mouse_coords.root = root_coords;

		return mouse_coords;
# endif
	}

	types::err_t de_init() {
		types::err_t any_err;
		if ((any_err = this->wd_handle.de_init()) != FFLY_SUCCESS) {
			fprintf(stderr, "window: failed to 'de init' window handler.\n");
			return any_err;
		}
		return FFLY_SUCCESS;
	}

	types::err_t nxt_event(types::event_desc_t& __event_desc, uint_t& __event_data) {
		if (this->ev_queue.empty()) {
			ffly_errno = FF_ERR_WDEQE;
			return FFLY_FAILURE;
		}
		__event_desc = this->ev_queue.front().first;
		__event_data = this->ev_queue.front().second;
		return FFLY_SUCCESS;
	}

	void event_pop() {this->ev_queue.pop();}

	_pointer_coords<i32_t, i32_t> pointer_coords;
    types::_2d_coords_t<i32_t> wd_coords;

	system::mq_mask<data::pair<types::event_desc_t, uint_t>> ev_queue;
	types::err_t init_report = FFLY_FAILURE;
	u16_t wd_xaxis_len = 0, wd_yaxis_len = 0;
	char const *frame_title = nullptr;
	public:
# if defined(USING_X11)
	x11_window wd_handle;
# elif defined(USING_XCB)
	xcb_window wd_handle;
# endif
};
}
}
}

# endif /*__window__hpp*/
