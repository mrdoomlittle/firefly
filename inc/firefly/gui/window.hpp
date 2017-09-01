# ifndef __gui__window__hpp
# define __gui__window__hpp
# include "wd_frame.hpp"
# include "../types/dsize_t.hpp"
# include "../types/coords_t.hpp"
# include "../memory/alloc_pixmap.hpp"
# include <errno.h>
# include "../system/errno.h"
# include "../memory/mem_alloc.h"
# include "../maths/is_inside.hpp"
# include "../types/err_t.h"
namespace mdl {
namespace firefly {
namespace gui {
class window {
	public:
	types::err_t init(u16_t __xaxis, u16_t __yaxis, u16_t __xaxis_len, u16_t __yaxis_len);
	void set_ptrs(bool *__mouse_press, int *__mouse_btn_id, types::coords_t<u16_t> *__mouse_coords) {
		this-> mouse_press = this-> _wd_frame.mouse_press = __mouse_press;
		this-> mouse_btn_id = this-> _wd_frame.mouse_btn_id = __mouse_btn_id;
		this-> mouse_coords = this-> _wd_frame.mouse_coords = __mouse_coords;
	}

	bool *mouse_press;
	int *mouse_btn_id;
	types::coords_t<u16_t> *mouse_coords;

	types::err_t draw(types::pixmap_t __pixbuff, types::dsize_t __pb_size);
	types::err_t handle();
	types::_1d_pm_t *get_wpm() {return this-> wpm;}

	types::err_t de_init() {
		if (this-> pixmap) {
			memory::mem_free(this-> pixmap);
			this-> pixmap = nullptr;
		} else
			printf("window: info, pixmap already freed. skipping - de_init.\n");

		if (this-> wpm) {
			memory::mem_free(this-> wpm);
			this-> wpm = nullptr;
		} else
			printf("window: info, wpm already freed. skipping - de_init.\n");
		return FFLY_SUCCESS;
	}

	void static exit_btn(void *_this);
	bool is_closed = false;
	types::coords_t<u16_t> pointer_coords;
	private:
	types::_1d_pm_t pixmap;
	// this will be added later
	types::_2d_pm_t _2d_wpm;
	types::_1d_pm_t *wpm;
	wd_frame _wd_frame;
	types::dsize_t pm_size;
	types::dsize_t wd_size;
	types::coords_t<u16_t> wd_coords;
	types::coords_t<u16_t> coords;
};
}
}
}


# endif /*__gui__window__hpp*/
