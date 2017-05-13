# ifndef __btn__manager__hpp
# define __btn__manager__hpp
# include "../types/coords_t.hpp"
# include "../types/pixmap_t.h"
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
# include <eint_t.hpp>
# include <boost/array.hpp>
# include "../types/btn_t.hpp"
# include "../types/btn_info_t.hpp"
# include "../memory/mem_free.h"
# include "../graphics/draw_pixmap.hpp"
# include "../types/id_t.hpp"
# include "../asset_manager.hpp"
# include "../memory/mem_alloc.h"
# include "../memory/alloc_pixmap.hpp"
# include "../graphics/fill_pixmap.hpp"
# include "../types/colour_t.hpp"
# include "../types/err_t.h"
# include "../font.hpp"
# include "../types/bitmap_t.hpp"
# include "../graphics/draw_bitmap.hpp"
# include "../types/colour_t.hpp"
# include "../types/btn_event_t.hpp"
# include "../system/event.hpp"
# include <queue>
# include "../types/err_t.h"
# include "../data/enable.hpp"
# include "../data/disable.hpp"
namespace mdl {
namespace firefly {
namespace gui {
class btn_manager {
	public:
	btn_manager(types::pixmap_t __pixbuff, types::coords_t<i16_t> *__wd_coords, types::coords_t<u16_t> *__mouse_coords, u16_t __pb_xaxis_len, u16_t __pb_yaxis_len)
	: pixbuff(__pixbuff), wd_coords(__wd_coords), mouse_coords(__mouse_coords), pb_xaxis_len(__pb_xaxis_len), pb_yaxis_len(__pb_yaxis_len) {
	}

	types::err_t create_btn(types::__id_t& __btn_id, types::_2d_coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len);
	uint_t create_btn(types::pixmap_t __pixmap, types::_2d_coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len);
	uint_t create_btn(types::id_t __asset_id, types::_2d_coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len, asset_manager *__asset_manger);

	uint_t create_btn(types::btn_info_t __btn_info) {
		return this-> create_btn(__btn_info.pixmap, __btn_info.coords, __btn_info.xaxis_len, __btn_info.yaxis_len);
	}

	types::err_t set_text(uint_t __btn_id, char const * __text, char const *__font,  uint_t __xoffset, uint_t __yoffset, std::size_t __spacing, bool __mid);

	void voidptr_lock(uint_t __btn_id) {
		this-> btn_index[__btn_id].voidptr_lock = true;}

	void static btn_press(uint_t __btn_id, int __mbtn_id, void *__voidptr);
	void static btn_hover(uint_t __btn_id, void *__voidptr);

	void set_voidptr(uint_t __btn_id, void *__ptr) {
		if (this-> btn_index[__btn_id].voidptr_lock) return;
		this-> btn_index[__btn_id].voidptr = __ptr;
	}

	void set_press_fptr(types::__id_t __btn_id, void (* __press_fptr)(uint_t, int, void *)) {
		this-> btn_index[__btn_id].press_fptr = __press_fptr;}

	void set_hover_fptr(types::__id_t __btn_id, void (* __hover_fptr)(uint_t, void *)) {
		this-> btn_index[__btn_id].hover_fptr = __hover_fptr;}

	types::err_t event_bind(types::__id_t __btn_id);

	bool mouse_inside(types::_2d_coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len);

	void enable_btn(types::__id_t __btn_id) {
		data::enable(this-> btn_index[__btn_id].enabled);}
	void disable_btn(types::__id_t __btn_id) {
		data::disable(this-> btn_index[__btn_id].enabled);}

	void enable_hover(types::__id_t __btn_id) {
		data::enable(this-> btn_index[__btn_id].hover_enabled);}
	void disable_hover(types::__id_t __btn_id) {
		data::disable(this-> btn_index[__btn_id].hover_enabled);}

	void enable_press(types::__id_t __btn_id) {
		data::enable(this-> btn_index[__btn_id].press_enabled);}

	bool mouse_hovering(uint_t __btn_id) {
		return this-> btn_index[__btn_id].mouse_hovering;}

	void set_pixbuff(types::pixmap_t __pixbuff) {
		this-> pixbuff = __pixbuff;}

	void set_pb_xlen(u16_t __pb_xaxis_len) {this-> pb_xaxis_len = __pb_xaxis_len;}
	void set_pb_ylen(u16_t __pb_yaxis_len) {this-> pb_yaxis_len = __pb_yaxis_len;}

	types::pixmap_t& btn_pixmap(uint_t __btn_id) {
		return btn_index[__btn_id].pixmap;}

	types::btn_t* get_btn(uint_t __btn_id) {
		return &this-> btn_index[__btn_id];}

	types::err_t manage(types::pixmap_t __pixbuff = nullptr);
	types::err_t de_init() {
		printf("btn_manager: going to free all btn pixmaps.\n");
		for (uint_t btn_id = 0; btn_id != this-> btn_index.size(); btn_id ++) {
			if (this-> btn_index[btn_id].texture != nullptr && !this-> btn_index[btn_id].inde_tx_mem)
				memory::mem_free(this-> btn_index[btn_id].texture);
			memory::mem_free(this-> btn_index[btn_id].text_bm);
		}
	}

	bool *mouse_pressed;
	int *mouse_btn_id;

	std::queue<types::btn_event_t> event_queue;
	types::coords_t<i16_t> *wd_coords;
    types::coords_t<u16_t> *mouse_coords;
	private:
	ublas::vector<types::btn_t> btn_index;
	types::pixmap_t pixbuff = nullptr;
	u16_t pb_xaxis_len, pb_yaxis_len;
};
}
}
}

# endif /*__btn__manager__hpp*/
