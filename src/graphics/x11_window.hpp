# ifndef __x11__window__hpp
# define __x11__window__hpp
# include <X11/Xlib.h>
# include <GL/glx.h>
# include <cstdio>
# include <chrono>
# include <eint_t.hpp>
//# include "/home/daniel-robson/Projects/eint_t/inc/eint_t.hpp"
# include <boost/thread.hpp>
# include <boost/cstdint.hpp>
# include "../memory/alloc_pixmap.hpp"
# include "../memory/mem_free.h"
# include "../types.hpp"
# include "../types/pixmap_t.h"
# define X11_WD_TO_CLOSE 16
# define X11_WD_CLOSED 1
# define X11_WD_OPEN 2
# define X11_WD_WAITING 4
# define X11_WD_DONE_DRAW 8
namespace mdl {
namespace firefly {
namespace graphics {
class x11_window
{
	public:
	boost::int8_t init(boost::uint16_t __wd_xaxis_len, boost::uint16_t __wd_yaxis_len, char const *__frame_title);
	boost::int8_t begin(boost::uint16_t __wd_xaxis_len, boost::uint16_t __wd_yaxis_len, char const *__frame_title);

	void set_fps_mark(std::size_t __fps_mark) {
		this-> fps_mark = __fps_mark;
	}

	window_coords_t window_coords;
	mouse_coords_t mouse_coords;

	types::pixmap_t pixbuff = nullptr;

	int button_code = 0x0;
	bool button_press = false;
	int key_code = 0x0;
	bool key_press = false;

	boost::int8_t wd_flags = X11_WD_CLOSED;
	bool is_wd_flag(boost::int8_t __wd_flag) {
		boost::int8_t to_compare = 0;

		to_compare = this-> wd_flags & __wd_flag;

		return to_compare == __wd_flag? true : false;
	}

	boost::uint16_t get_wd_xaxis_len() {
		return this-> wd_xaxis_len;
	}

	boost::uint16_t get_wd_yaxis_len() {
		return this-> wd_yaxis_len;
	}

	char const* get_frame_title() {
		return this-> frame_title;
	}

	void add_wd_flag(boost::int8_t __wd_flag, bool __over_write = false) {
		if (__over_write) {
			this-> wd_flags = __wd_flag;
			return;
		}

		this-> wd_flags |= __wd_flag;
	}

	void rm_wd_flag(boost::int8_t __wd_flag) {
		this-> wd_flags ^= this-> wd_flags & __wd_flag;
	}

	private:
	boost::uint16_t wd_xaxis_len, wd_yaxis_len;
	char const *frame_title;
	std::size_t fps_mark = 120;
} ;
}
}
}

# endif /*__x11__window__hpp*/
