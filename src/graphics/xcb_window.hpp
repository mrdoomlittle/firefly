# ifndef __xcb__window__hpp
# define __xcb__window__hpp
# include <cstdio>
# include <cstdlib>
# include <X11/Xlib.h>
# include <X11/Xlib-xcb.h>
# include <unistd.h>
# include <xcb/xcb.h>
# include <GL/glx.h>
# include <GL/gl.h>
# include <boost/cstdint.hpp>
# include "../types/pixmap_t.h"
# include <eint_t.hpp>
# include "../types/status.hpp"
namespace mdl {
namespace firefly {
namespace graphics {
class xcb_window
{
	public:
	boost::int8_t init(boost::uint16_t __wd_xaxis_len, boost::uint16_t __wd_yaxis_len, char const *__frame_title);
	//boost::int8_t begin(uint_t __wd_xlen, uint_t __wd_ylen, char const *__frame_title);

	types::pixmap_t pixbuff = nullptr;
};
}
}
}


# endif /*__xcb__window__hpp*/
