# include "btn_manager.hpp"

bool mdl::firefly::gui::btn_manager::mouse_inside(types::_2d_coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len) {
	if ((this-> mouse_coords-> xaxis > __coords.xaxis && this-> mouse_coords-> xaxis <= (__coords.xaxis + __xaxis_len))
	&& (this-> mouse_coords-> yaxis > __coords.yaxis && this-> mouse_coords-> yaxis <= (__coords.yaxis + __yaxis_len))) return true;
	return false;
}

void mdl::firefly::gui::btn_manager::btn_press(uint_t __btn_id, int __mbtn_id, void *__voidptr) {
	types::btn_event_t btn_event = {
		.btn_id = __btn_id,
		.mbtn_id = __mbtn_id,
		.event_type = system::event::BTN_PRESS
	};

	((btn_manager *)__voidptr)-> event_queue.push(btn_event);
}

void mdl::firefly::gui::btn_manager::btn_hover(uint_t __btn_id, void *__voidptr) {
	types::btn_event_t btn_event = {
		.btn_id = __btn_id,
		.mbtn_id = 0,
		.event_type = system::event::BTN_HOVER
	};

	((btn_manager *)__voidptr)-> event_queue.push(btn_event);
}

mdl::firefly::types::err_t mdl::firefly::gui::btn_manager::event_bind(uint_t __btn_id) {
	this-> set_press_fptr(__btn_id, &btn_manager::btn_press);
	this-> set_hover_fptr(__btn_id, &btn_manager::btn_hover);

	this-> set_voidptr(__btn_id, this);
	this-> voidptr_lock(__btn_id);

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::gui::btn_manager::create_btn(uint_t& __btn_id, types::_2d_coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len) {
	types::pixmap_t btn_pixmap;
	if ((btn_pixmap = memory::alloc_pixmap(__xaxis_len, __yaxis_len)) == NULL) {
		fprintf(stderr, "btn_manager: failed to alloc memory for btn pixmap, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	if (graphics::fill_pixmap(btn_pixmap, __xaxis_len, __yaxis_len, graphics::__colour__(0, 0, 0, 254)) == FFLY_FAILURE) {
		fprintf(stderr, "btn_manager: failed to fill in btn pixmap.\n");
		return FFLY_FAILURE;
	}

	__btn_id = this-> create_btn(btn_pixmap, types::__coords__<>(__coords.xaxis, __coords.yaxis), __xaxis_len, __yaxis_len);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::gui::btn_manager::set_text(uint_t __btn_id, char const *__text, char const *__font,  uint_t __xoffset, uint_t __yoffset, std::size_t __spacing, bool __mid) {
	font _font;
	if (_font.init() == FFLY_FAILURE) return FFLY_FAILURE;

	if (_font.load_font("", __font) == FFLY_FAILURE)
		return FFLY_FAILURE;

	_font.set_size(16);

	types::btn_t *btn = this-> get_btn(__btn_id);
	if (btn-> text_bm != nullptr) {
		memory::mem_free(btn-> text_bm);
		btn-> text_bm = nullptr;
	}

	btn-> text_bm = _font.make_bitmap(__text, __spacing, btn-> xaxis_len, btn-> yaxis_len);
	if (btn-> text_bm == NULL)
		return FFLY_FAILURE;

	if (__mid)
		_font.center(btn-> text_coords.xaxis, btn-> text_coords.yaxis);
	else {
		btn-> text_coords.xaxis = __xoffset;
		btn-> text_coords.yaxis = __yoffset;
	}

	btn-> draw_text_bm = true;
	btn-> text_colour.r = 0;
	btn-> text_colour.g = 0;
	btn-> text_colour.b = 0;
	btn-> text_colour.a = 254;

	return FFLY_SUCCESS;
	//graphics::draw_bitmap(btn-> text_bm, btn-> xaxis_len, btn-> yaxis_len, btn-> pixmap, btn-> xaxis_len, btn-> yaxis_len, _font.center(), graphics::__colour__(0, 0, 0, 244));

	//memory::mem_free(btn-> text_bm);
}

// this need no err return for now
mdl::uint_t mdl::firefly::gui::btn_manager::create_btn(types::pixmap_t __pixmap, types::_2d_coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len) {
//	printf("btn_manager: create btn %d\n", this-> btn_index.size())
	uint_t btn_id = this-> btn_index.size();
	this-> btn_index.resize(this-> btn_index.size() + 1);

	types::btn_t btn;
	btn.coords = __coords;
	btn.xaxis_len = __xaxis_len;
	btn.yaxis_len = __yaxis_len;

	btn.pixmap = (types::pixmap_t)memory::mem_alloc((__xaxis_len * __yaxis_len) * 4);
	btn.texture = __pixmap;

	btn.hover_fptr = nullptr;
	btn.press_fptr = nullptr;
	btn.enabled = false;
	btn.hover_enabled = false;
	btn.press_enabled = false;
	btn.text_bm = nullptr;
	btn.inde_tx_mem = false;
	this-> btn_index(btn_id) = btn;
	return btn_id;
}

mdl::uint_t mdl::firefly::gui::btn_manager::create_btn(types::id_t __asset_id, types::_2d_coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len, asset_manager *__asset_manager) {
	uint_t btn_id = this-> create_btn(__asset_manager-> get_asset_data(__asset_id), __coords, __xaxis_len, __yaxis_len);
	this-> btn_index[btn_id].inde_tx_mem = true;
	return btn_id;
}

boost::int8_t mdl::firefly::gui::btn_manager::manage(types::pixmap_t __pixbuff) {
	if (this-> btn_index.size() == 0) return FFLY_NOP;
	uint_t btn_id = 0;
	auto update = [&btn_id]() {
		btn_id ++;
	};

	while (btn_id != this-> btn_index.size()) {
		types::btn_t& btn = this-> btn_index[btn_id];
		if (!btn.enabled || (__pixbuff == nullptr && this-> pixbuff == nullptr)) {
			update();
			continue;
		}

		if (graphics::draw_pixmap(0, 0, btn.pixmap, btn.xaxis_len, btn.yaxis_len, btn.texture, btn.xaxis_len, btn.yaxis_len) != FFLY_SUCCESS) return FFLY_FAILURE;

		if (btn.draw_text_bm && btn.text_bm != nullptr) {
			if (graphics::draw_bitmap(btn.text_bm, btn.xaxis_len, btn.yaxis_len, btn.pixmap, btn.xaxis_len, btn.yaxis_len, btn.text_coords, btn.text_colour)) return FFLY_FAILURE;
		}

		if (graphics::draw_pixmap(btn.coords.xaxis, btn.coords.yaxis, __pixbuff != nullptr? __pixbuff : this-> pixbuff, this-> pb_xaxis_len, this-> pb_yaxis_len, btn.pixmap, btn.xaxis_len, btn.yaxis_len) != FFLY_SUCCESS) return FFLY_FAILURE;

		if (btn.hover_enabled) {
			if (this-> mouse_inside(btn.coords, btn.xaxis_len, btn.yaxis_len)) {
				if (btn.hover_fptr != nullptr)
					btn.hover_fptr(btn_id, btn.voidptr);

				btn.mouse_hovering = true;
			} else
				btn.mouse_hovering = false;
		}

		if (btn.press_enabled) {
			if (btn.mouse_hovering) {
				if (*this-> mouse_pressed) {
					if (!btn.already_pressed)
						if (btn.press_fptr != nullptr)
							btn.press_fptr(btn_id, *this-> mouse_btn_id, btn.voidptr);

					btn.already_pressed = true;
				} else
					btn.already_pressed = false;
			}
		}

		update();
	}

	return FFLY_SUCCESS;
}
