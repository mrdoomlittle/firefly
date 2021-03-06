# ifndef __xcb__window__hpp
# define __xcb__window__hpp
# include <cstdio>
# include <cstdlib>
# include <X11/Xlib.h>
# include <X11/Xlib-xcb.h>
# include <xcb/xproto.h>
# include <unistd.h>
# include <xcb/xcb.h>
# include <xcb/xcb_icccm.h>
# include <GL/glx.h>
# include <GL/gl.h>
# include <boost/cstdint.hpp>
# include "../types/pixmap_t.h"
# include <mdlint.h>
# include <string.h>
# include <boost/thread.hpp>
# include "../types/status.hpp"
# include "../types/coords_t.hpp"
# include "../types/mouse_coords_t.hpp"
# include "../memory/alloc_pixmap.hpp"
# include "../memory/mem_free.h"
# include "wd_flags.hpp"
# include "../system/stop_watch.hpp"
# include "../types/err_t.h"
/*do not include graphics/window.hpp in this header file*/
namespace mdl {
namespace firefly {
namespace graphics {
class window;
class xcb_window: public wd_flags
{
	friend class window;
	public:
	types::err_t begin(u16_t__wd_xaxis_len, u16_t__wd_yaxis_len, char const *__frame_title);
	types::err_t open_in_thread(u16_t__wd_xaxis_len, u16_t__wd_yaxis_len, char const *__frame_title);

	void set_fps_mark(std::size_t __fps_mark) {
		this-> fps_mark = __fps_mark;
	}

	types::coords_t<boost::int16_t> wd_coords;
	types::mouse_coords_t<boost::int16_t> mouse_coords;
	bool contains_pointer = false;

	bool key_press = false;
	int key_code = 0x0;

	bool button_press = false;
	int button_code = 0x0;

	u16_tget_wd_xaxis_len() {
		return this-> wd_xaxis_len;
	}

	u16_tget_wd_yaxis_len() {
		return this-> wd_yaxis_len;
	}

	char *get_frame_title() {
		return this-> frame_title;
	}

	types::pixmap_t pixbuff = nullptr;

	types::err_t de_init() {
		this-> add_wd_flag(FLG_WD_TO_CLOSE);

		while(!this-> is_wd_flag(FLG_WD_CLOSED)){}

		memory::mem_free(pixbuff);
		memory::mem_free(frame_title);
/*
		int th_err = 0;
		if ((th_err = pthread_cancel(this-> native_handle)) != 0) {
			if (th_err != ESRCH) {
				fprintf(stderr, "xcb_window: failed to cancel posix thread.\n");
				return FFLY_FAILURE;
			}
		}
*/
		return FFLY_SUCCESS;
	}

	private:
	boost::thread::native_handle_type native_handle;
	u16_twd_xaxis_len, wd_yaxis_len;
	char *frame_title;
	std::size_t fps_mark = 120;
};
}
}
}


# endif /*__xcb__window__hpp*/
