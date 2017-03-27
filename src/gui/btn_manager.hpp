# ifndef __btn__manager__hpp
# define __btn__manager__hpp
# include "../types/coords_t.hpp"
# include "../types/pixmap_t.h"
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
# include <eint_t.hpp>
# include <boost/array.hpp>
# include "../types/btn_info_t.hpp"
# include "../graphics/draw_pixmap.hpp"
namespace mdl {
namespace firefly {
namespace gui {
class btn_manager {
	public:
	btn_manager(types::pixmap_t __pixbuff, types::coords_t<boost::int16_t> *__wd_coords, types::coords_t<boost::uint16_t> *__mouse_coords, boost::uint16_t __pb_xaxis_len, boost::uint16_t __pb_yaxis_len)
	: pixbuff(__pixbuff), wd_coords(__wd_coords), mouse_coords(__mouse_coords), pb_xaxis_len(__pb_xaxis_len), pb_yaxis_len(__pb_yaxis_len) {}

	uint_t create_btn(types::pixmap_t __pixmap, types::coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len);

	void set_press_fptr(uint_t __btn_id, void (* __press_fptr)(uint_t, int)) {
		this-> btn_index[__btn_id].press_fptr = __press_fptr;
	}

	void set_hover_fptr(uint_t __btn_id, void (* __hover_fptr)(uint_t)) {
		this-> btn_index[__btn_id].hover_fptr = __hover_fptr;
	}

	bool mouse_inside(types::coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len);
	void enable_btn(uint_t __btn_id) {
		this-> btn_index[__btn_id].enabled = true;
	}

	void disable_btn(uint_t __btn_id) {
		this-> btn_index[__btn_id].enabled = false;
	}

	void enable_hover(uint_t __btn_id) {
		this-> btn_index[__btn_id].hover_enabled = true;
	}

	void disable_hover(uint_t __btn_id) {
		this-> btn_index[__btn_id].hover_enabled = false;
	}

	bool mouse_hovering(uint_t __btn_id) {
		return this-> btn_index[__btn_id].mouse_hovering;
	}

	void set_pixbuff(types::pixmap_t __pixbuff) {
		this-> pixbuff = __pixbuff;
	}

	void set_pb_xlen(boost::uint16_t __pb_xaxis_len) {
		this-> pb_xaxis_len = __pb_xaxis_len;
	}

	void set_pb_ylen(boost::uint16_t __pb_yaxis_len) {
		this-> pb_yaxis_len = __pb_yaxis_len;
	}

	boost::int8_t manage(types::pixmap_t __pixbuff = nullptr);

	bool *mouse_pressed;
	int *mouse_btn_id;
	private:
	ublas::vector<types::btn_info_t> btn_index;
	types::pixmap_t pixbuff = nullptr;
	types::coords_t<boost::int16_t> *wd_coords;
	types::coords_t<boost::uint16_t> *mouse_coords;
	boost::uint16_t pb_xaxis_len, pb_yaxis_len;
};
}
}
}

# endif /*__btn__manager__hpp*/
