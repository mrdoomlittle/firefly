# ifndef __x11__window__hpp
# define __x11__window__hpp
# include <X11/Xlib.h>
# include <GL/glx.h>
# include <cstdio>
# include <chrono>
# include "/home/daniel-robson/Projects/eint_t/inc/eint_t.hpp"
# include <boost/thread.hpp>
# include <boost/cstdint.hpp>
# include "types.hpp"
namespace mdl { class x11_window
{
	public:
	boost::uint8_t init(boost::uint16_t __wx_axis_len, boost::uint16_t __wy_axis_len, char const *__title);
	boost::uint8_t begin(boost::uint16_t __wx_axis_len, boost::uint16_t __wy_axis_len, char const *__title);

	bool init_called = false;


	std::size_t fps_mark = 60;
	window_coords_t window_coords;
	mouse_coords_t mouse_coords;
	boost::uint8_t * pixels = nullptr;

	int button_code = 0x0;
	bool button_press = false;
	int key_code = 0x0;
	bool key_press = false;
	bool waitting = false;
	bool done_drawing = false;
	bool reading_stuff = false;

	private:
	boost::thread * window_thread;
	boost::uint16_t __wx_axis_len;
	boost::uint16_t __wy_axis_len;
	char const *__title;
} ;
}

# endif /*__x11__window__hpp*/
