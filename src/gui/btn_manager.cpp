# include "btn_manager.hpp"

bool mdl::firefly::gui::btn_manager::mouse_inside(types::coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len) {
	if ((this-> mouse_coords-> xaxis > __coords.xaxis && this-> mouse_coords-> xaxis <= (__coords.xaxis + __xaxis_len))
	&& (this-> mouse_coords-> yaxis > __coords.yaxis && this-> mouse_coords-> yaxis <= (__coords.yaxis + __yaxis_len))) return true;
	return false;
}

mdl::uint_t mdl::firefly::gui::btn_manager::create_btn(types::pixmap_t __pixmap, types::coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len) {
//	printf("btn_manager: create btn %d\n", this-> btn_index.size())
	uint_t btn_id = this-> btn_index.size();
	this-> btn_index.resize(this-> btn_index.size() + 1);

	types::btn_t btn_info;

	btn_info.coords = __coords;
	btn_info.xaxis_len = __xaxis_len;
	btn_info.yaxis_len = __yaxis_len;

	btn_info.pixmap = __pixmap;

	btn_info.hover_fptr = nullptr;
	btn_info.press_fptr = nullptr;
	btn_info.enabled = true;
	btn_info.hover_enabled = false;
	btn_info.press_enabled = false;
	btn_info.inde_pm_mem = false;
	this-> btn_index(btn_id) = btn_info;
	return btn_id;
}

mdl::uint_t mdl::firefly::gui::btn_manager::create_btn(types::id_t __asset_id, types::coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len, asset_manager *__asset_manager) {
	uint_t btn_id = this-> create_btn(__asset_manager-> get_asset_data(__asset_id), __coords, __xaxis_len, __yaxis_len);
	this-> btn_index[btn_id].inde_pm_mem = true;
	return btn_id;
}

boost::int8_t mdl::firefly::gui::btn_manager::manage(types::pixmap_t __pixbuff) {
	if (this-> btn_index.size() == 0) return FFLY_NOP;
	uint_t btn_id = 0;
	auto update = [&btn_id]() {
		btn_id ++;
	};

	while (btn_id != this-> btn_index.size()) {
		types::btn_t& btn_info = this-> btn_index[btn_id];
		if (!btn_info.enabled || (__pixbuff == nullptr && this-> pixbuff == nullptr)) {
			update();
			continue;
		}

		if (graphics::draw_pixmap(btn_info.coords.xaxis, btn_info.coords.yaxis, __pixbuff != nullptr? __pixbuff : this-> pixbuff, this-> pb_xaxis_len, this-> pb_yaxis_len, btn_info.pixmap, btn_info.xaxis_len, btn_info.yaxis_len) != FFLY_SUCCESS) return FFLY_FAILURE;

		if (btn_info.hover_enabled) {
			if (this-> mouse_inside(btn_info.coords, btn_info.xaxis_len, btn_info.yaxis_len)) {
				if (btn_info.hover_fptr != nullptr)
					btn_info.hover_fptr(btn_id, btn_info.voidptr);

				btn_info.mouse_hovering = true;
			} else
				btn_info.mouse_hovering = false;
		}

		if (btn_info.press_enabled) {
			if (btn_info.mouse_hovering) {
				if (*this-> mouse_pressed) {
					if (!btn_info.already_pressed)
						if (btn_info.press_fptr != nullptr)
							btn_info.press_fptr(btn_id, *this-> mouse_btn_id, btn_info.voidptr);

					btn_info.already_pressed = true;
				} else
					btn_info.already_pressed = false;
			}
		}

		update();
	}

	return FFLY_SUCCESS;
}
