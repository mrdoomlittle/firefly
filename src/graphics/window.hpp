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
# include "../system/mem_blk.h"
# include "wd_flags.h"
namespace mdl {
namespace firefly {
namespace graphics {
class window {
	public:
	types::byte_t* frame_buff() {return ffly_wd_frame_buff(&this->raw);}
	types::err_t init(u16_t __width, u16_t __height, char const *__title);
	types::err_t begin();
	types::bool_t is_dead() {return system::is_flag(this->raw.flags, FFLY_FLG_WD_DEAD);}
	types::bool_t is_alive() {return system::is_flag(this->raw.flags, FFLY_FLG_WD_ALIVE);}
	types::flag_t& flags(){return *ffly_wd_flags(&this->raw);}
	void update();
	types::bool_t is_event_buff_empty() {return !ffly_buff_ublk_c(&this->raw.event_buff);}
	types::err_t dump_event_buff();
	types::err_t de_init();
	system::queue<types::event_t> event_queue;
	void free_event_data(void *__data) {
		ffly_mem_blk_free(&this->raw.event_data, __data);
	}
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
