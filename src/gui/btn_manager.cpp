# include "btn_manager.hpp"

bool mdl::firefly::gui::btn_manager::mouse_inside(types::coords_t __coords, uint_t __xaxis_len, uint_t __yaxis_len) {
	if ((this-> mouse_coords-> xaxis > __coords.xaxis && this-> mouse_coords-> xaxis < (__coords.xaxis + __xaxis_len))
	&& (this-> mouse_coords-> yaxis > __coords.yaxis && this-> mouse_coords-> yaxis < (__coords.yaxis + __yaxis_len))) return true;
	return false;
}

mdl::uint_t mdl::firefly::gui::btn_manager::create_btn(types::pixmap_t __pixmap, types::coords_t __coords, uint_t __xaxis_len, uint_t __yaxis_len) {
	uint_t btn_id = this-> btn_index.size();
	this-> btn_index.resize(this-> btn_index.size() + 1);

	types::btn_info_t btn_info;

	btn_info.coords = __coords;
	btn_info.xaxis_len = __xaxis_len;
	btn_info.yaxis_len = __yaxis_len;

	btn_info.pixmap = __pixmap;

	btn_info.enabled = false;
	btn_info.hover_enabled = false;

	this-> btn_index(btn_id) = btn_info;
}

boost::int8_t mdl::firefly::gui::btn_manager::manage(types::pixmap_t __pixbuff) {
	uint_t btn_id = 0;
	auto update = [&btn_id]() {
		btn_id ++;
	};

	while (btn_id != this-> btn_index.size()) {
		types::btn_info_t& btn_info = this-> btn_index[btn_id];
		if (!btn_info.enabled) {
			update();
			continue;
		}

		if (graphics::draw_pixmap(btn_info.coords.xaxis, btn_info.coords.yaxis, __pixbuff != nullptr? __pixbuff : this-> pixbuff, this-> pb_xaxis_len, this-> pb_yaxis_len, btn_info.pixmap, btn_info.xaxis_len, btn_info.yaxis_len) != FFLY_SUCCESS) return FFLY_FAILURE;

		if (btn_info.hover_enabled) {
				if (this-> mouse_inside(btn_info.coords, btn_info.xaxis_len, btn_info.yaxis_len))
					if (btn_info.hover_fptr != nullptr) {
						btn_info.hover_fptr(btn_id);
					btn_info.mouse_hovering = true;
				} else
					btn_info.mouse_hovering = false;
		}

		update();
	}
}