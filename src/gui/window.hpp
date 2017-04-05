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
namespace mdl {
namespace firefly {
namespace gui {
class window {
	public:

	boost::int8_t init(boost::uint16_t __xaxis, boost::uint16_t __yaxis, boost::uint16_t __xaxis_len, boost::uint16_t __yaxis_len);

	void set_ptrs(bool *__mouse_press, int *__mouse_btn_id, types::coords_t<boost::uint16_t> *__mouse_coords) {
		this-> mouse_press = this-> _wd_frame.mouse_press = __mouse_press;
		this-> mouse_btn_id = this-> _wd_frame.mouse_btn_id = __mouse_btn_id;
		this-> mouse_coords = this-> _wd_frame.mouse_coords = __mouse_coords;
	}

	bool *mouse_press;
	int *mouse_btn_id;
	types::coords_t<boost::uint16_t> *mouse_coords;

	boost::int8_t draw(types::pixmap_t __pixbuff, types::dsize_t __pb_size);
	boost::int8_t handle();

	boost::int8_t de_init() {
		if (this-> pixmap != nullptr) {
			memory::mem_free(this-> pixmap);
			this-> pixmap = nullptr;
		}

		if (this-> wpixmap != nullptr) {
			memory::mem_free(this-> wpixmap);
			this-> wpixmap = nullptr;
		}
	}

	static void exit_btn(void *_this);
	bool is_closed = false;
	types::coords_t<boost::uint16_t> pointer_coords;
	private:
	types::pixmap_t pixmap;
	types::pixmap_t *wpixmap;
	wd_frame _wd_frame;
	types::dsize_t pm_size;
	types::dsize_t wd_size;
	types::coords_t<boost::uint16_t> wd_coords;
	types::coords_t<boost::uint16_t> coords;
};
}
}
}


# endif /*__gui__window__hpp*/
