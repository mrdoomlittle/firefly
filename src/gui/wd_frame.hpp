# ifndef __wd__frame__hpp
# define __wd__frame__hpp
# include <boost/cstdint.hpp>
# include "btn_manager.hpp"
# include "../types/pixmap_t.h"
# include "../types/dsize_t.hpp"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/alloc_pixmap.hpp"
# include "../graphics/draw_outline.hpp"
# include "../system/errno.h"
# include "../graphics/draw_pixmap.hpp"
# include <errno.h>
# include "../types/err_t.h"
# include "../maths/is_inside.hpp"
# define OUTLINE_WIDTH 14
namespace mdl {
namespace firefly {
namespace gui {
class wd_frame {
	public:
	types::err_t init(boost::uint16_t __xaxis, boost::uint16_t __yaxis, boost::uint16_t __xaxis_len, boost::uint16_t __yaxis_len, char const *__title);

	types::err_t draw(types::pixmap_t __pixbuff, types::dsize_t __pb_size);
	types::err_t handle();
	bool *mouse_press;
	int *mouse_btn_id;

	bool is_inside();

	void set_pixmap(types::pixmap_t __pixmap) {
		this-> pixmap = __pixmap;
	}

	types::coords_t<boost::uint16_t> get_coords() {
		return this-> coords;
	}

	types::err_t de_init() {
		if (this-> pixmap != nullptr) {
			memory::mem_free(this-> pixmap);
			this-> pixmap = nullptr;
		}
	}

	void (* ex_fptr)(void *) = nullptr;
	void *ex_arg = nullptr;

	types::coords_t<boost::uint16_t> *mouse_coords = nullptr;
	private:
	types::coords_t<boost::uint8_t> ex_btn_coords;
	types::dsize_t ex_btn_size;
	types::coords_t<boost::uint16_t> coords;
	types::dsize_t pm_size = {0, 0, 0};
	types::dsize_t pb_size = {0, 0, 0};
	types::pixmap_t pixmap = nullptr;
};

}
}
}

# endif /*__wd__frame__hpp*/
