# ifndef __window__hpp
# define __window__hpp
# if defined(__USING_X11)
#	include "x11_wd.h"
# elif defined(__USING_XCB)
#	include "xcb_wd.h"
# endif
# include <mdlint.h>
# include "../types/err_t.h"
# include "../system/errno.h"
# include "../system/io.h"
# include "../system/thread.h"
# include "../types/byte_t.h"
# include "../system/flags.h"
# include "../system/queue.h"
# include "../types/bool_t.h"
# include "../types/event_t.h"
namespace mdl {
namespace firefly {
namespace graphics {
class window {
	public:
	types::byte_t* frame_buff() {return this->handle.frame_buff;}
	types::err_t init(u16_t __xa_len, u16_t __ya_len, char const *__title);
	types::err_t begin();
	types::flag_t& flags(){return this->handle.flags;}
	types::bool_t is_event_buff_empty() {return ffly_buff_ublk_c(&this->handle.event_buff) == 0;}
	types::err_t dump_event_buff();
	private:
	system::queue<types::event_t> event_queue;
	char const *title;
	u16_t xa_len, ya_len;
# if defined(__USING_X11)
	struct ffly_x11_wd handle;
# elif defined(__USING_XCB)
	struct ffly_xcb_wd handle;
# endif
};
}
}
}

# endif /*__window__hpp*/
