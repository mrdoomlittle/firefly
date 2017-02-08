# ifndef __x11__window__hpp
# define __x11__window__hpp
# include <X11/Xlib.h>
# include <GL/glx.h>
# include <cstdio>
# include "/home/daniel-robson/Projects/eint_t/inc/eint_t.hpp"
# include <boost/thread.hpp>
# include <boost/cstdint.hpp>
namespace mdl { class x11_window
{
	public:
	boost::uint8_t init(boost::uint16_t __wx_axis_len, boost::uint16_t __wy_axis_len, char const *__title);
	boost::uint8_t begin(boost::uint16_t __wx_axis_len, boost::uint16_t __wy_axis_len, char const *__title);

	bool init_called = false;
	typedef struct {
		int r_xaxis, r_yaxis;
		int s_xaxis, s_yaxis;
		int w_xaxis, w_yaxis;
	} mouse_coords_t;

	typedef struct {
		int xaxis, yaxis;
	} window_coords_t;

	window_coords_t window_coords;
	mouse_coords_t mouse_coords;
	boost::uint8_t * pixels = nullptr;

	private:
	boost::thread * window_thread;
	boost::uint16_t __wx_axis_len;
	boost::uint16_t __wy_axis_len;
	char const *__title;
} ;
}

# endif /*__x11__window__hpp*/
