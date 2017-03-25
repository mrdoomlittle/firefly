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
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../types.hpp"
# include "../types/pixmap_t.h"
# include "../types/coords_t.hpp"
# include "../types/mouse_coords_t.hpp"
# include "../system/errno.h"
# include "wd_flags.hpp"
# include "../system/stop_watch.hpp"
namespace mdl {
namespace firefly {
namespace graphics {
class x11_window: public wd_flags
{
	public:
	boost::int8_t begin(boost::uint16_t __wd_xaxis_len, boost::uint16_t __wd_yaxis_len, char const *__frame_title);
	boost::int8_t open_in_thread(boost::uint16_t __wd_xaxis_len, boost::uint16_t __wd_yaxis_len, char const *__frame_title);

	void set_fps_mark(std::size_t __fps_mark) {
		this-> fps_mark = __fps_mark;
	}

	types::coords_t<int> wd_coords;
	types::mouse_coords_t<int, int> mouse_coords;
	bool contains_pointer = false;

	types::pixmap_t pixbuff = nullptr;

	bool key_press = false;
	int key_code = 0x0;

	bool button_press = false;
	int button_code = 0x0;

	boost::uint16_t get_wd_xaxis_len() {
		return this-> wd_xaxis_len;
	}

	boost::uint16_t get_wd_yaxis_len() {
		return this-> wd_yaxis_len;
	}

	char * get_frame_title() {
		return this-> frame_title;
	}

	private:
	boost::uint16_t wd_xaxis_len, wd_yaxis_len;
	char *frame_title;
	std::size_t fps_mark = 120;
} ;
}
}
}

# endif /*__x11__window__hpp*/
