# ifndef __window__hpp
# define __window__hpp
# include "window.h"
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
	types::byte_t* frame_buff() {return ffly_wd_frame_buff(&this->raw);}
    types::err_t query_pointer(i16_t& __root_xa, i16_t& __root_ya, i16_t& __wd_xa, i16_t& __wd_ya);
	types::err_t init(u16_t __width, u16_t __height, char const *__title);
	types::err_t open() {return ffly_wd_open(&this->raw);}
	types::err_t close() {return ffly_wd_close(&this->raw);}
	types::err_t cleanup() {return ffly_wd_cleanup(&this->raw);}
	types::bool_t poll_event(types::event_t*& __event, types::err_t& __err);
	types::err_t free_event(types::event_t *__event);
	types::err_t display(){return ffly_wd_display(&this->raw);}
	types::err_t de_init();
	u16_t get_width() {return this->width;}
	u16_t get_height() {return this->height;}
	private:
	struct ffly_wd raw;
	char const *title;
	u16_t width, height;
	types::tid_t tid;
};
}
}
}

# endif /*__window__hpp*/
