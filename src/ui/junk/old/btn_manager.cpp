# include "btn_manager.hpp"
bool mdl::firefly::gui::btn_manager::mouse_inside(types::_2d_coords_t __coords, uint_t __xa_len, uint_t __ya_len) {
	if ((this->mouse_coords->xaxis > __coords.xaxis && this->mouse_coords->xaxis <= __coords.xa+__xa_len)
	&& (this->mouse_coords->yaxis > __coords.yaxis && this->mouse_coords->yaxis <= __coords.ya+__ya_len)) return true;
	return false;
}

bool ev_queue_mx(mdl::uint_t __size) {
	bool res = __size >= mdl::firefly::system::config.mx_queue_size? true : false;
	printf("btn_manager: info, btn event queue is over the global max limit, no more events will be captured untill queue size decreases.\n");
	return res;
}

void mdl::firefly::gui::btn_manager::btn_press(types::__id_t __btn_id, int __mbtn_id, void *__voidptr) {
	if (ev_queue_mx(((btn_manager *)__voidptr)->event_queue.size())) return;
	types::btn_event_t btn_event = {.btn_id = __btn_id, .mbtn_id = __mbtn_id};
	((btn_manager *)__voidptr)->event_queue.push(data::__pair__<types::btn_event_t, uint_t>(btn_event, system::GUI_BTN_PRESS));
}

void mdl::firefly::gui::btn_manager::btn_hover(types::__id_t __btn_id, void *__voidptr) {
	if (ev_queue_mx(((btn_manager *)__voidptr)->event_queue.size())) return;
	types::btn_event_t btn_event = {.btn_id = __btn_id, .mbtn_id = 0};
	((btn_manager *)__voidptr)->event_queue.push(data::__pair__<types::btn_event_t, uint_t>(btn_event, system::GUI_BTN_HOVER));
}

mdl::firefly::types::err_t mdl::firefly::gui::btn_manager::event_bind(types::__id_t __btn_id) {
	this->set_press_fptr(__btn_id, &btn_manager::btn_press);
	this->set_hover_fptr(__btn_id, &btn_manager::btn_hover);

	this->set_voidptr(__btn_id, this);
	this->voidptr_lock(__btn_id);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::gui::btn_manager::create_btn(types::__id_t& __btn_id, types::_2d_coords_t __coords, uint_t __xaxis_len, uint_t __yaxis_len) {
	types::pixmap_t btn_pixmap;
	if ((btn_pixmap = memory::alloc_pixmap(__xaxis_len, __yaxis_len)) == NULL) {
		fprintf(stderr, "btn_manager: failed to alloc memory for btn pixmap, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	if (graphics::fill_pixmap(btn_pixmap, __xaxis_len, __yaxis_len, graphics::__colour__(0, 0, 0, 254)) == FFLY_FAILURE) {
		fprintf(stderr, "btn_manager: failed to fill in btn pixmap.\n");
		return FFLY_FAILURE;
	}

	__btn_id = this->create_btn(btn_pixmap, types::__coords__<>(__coords.xaxis, __coords.yaxis), __xaxis_len, __yaxis_len);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::gui::btn_manager::set_text(types::__id_t __btn_id, char const *__text, char const *__font,  uint_t __xoffset, uint_t __yoffset, std::size_t __spacing, bool __mid) {
	font _font;
	if (_font.init() == FFLY_FAILURE) return FFLY_FAILURE;

	if (_font.load_font("", __font) == FFLY_FAILURE)
		return FFLY_FAILURE;

	_font.set_size(16);

	types::btn_t *btn = this->get_btn(__btn_id);
	if (btn->text_bm) {
		memory::mem_free(btn->text_bm);
		btn->text_bm = nullptr;
	}

	btn->text_bm = _font.make_bitmap(__text, __spacing, btn->xaxis_len, btn->yaxis_len);
	if (btn->text_bm == NULL)
		return FFLY_FAILURE;

	if (__mid)
		_font.center(btn->text_coords.xaxis, btn->text_coords.yaxis);
	else {
		btn->text_coords.xaxis = __xoffset;
		btn->text_coords.yaxis = __yoffset;
	}

	btn->draw_text_bm = true;
	btn->text_colour.r = 0;
	btn->text_colour.g = 0;
	btn->text_colour.b = 0;
	btn->text_colour.a = 254;

	return FFLY_SUCCESS;
	//graphics::draw_bitmap(btn->text_bm, btn->xaxis_len, btn->yaxis_len, btn->pixmap, btn->xaxis_len, btn->yaxis_len, _font.center(), graphics::__colour__(0, 0, 0, 244));

	//memory::mem_free(btn->text_bm);
}

// this need no err return for now
mdl::uint_t mdl::firefly::gui::btn_manager::create_btn(types::pixmap_t __pixmap, types::_2d_coords_t __coords, uint_t __xaxis_len, uint_t __yaxis_len) {
//	printf("btn_manager: create btn %d\n", this->btn_index.size())
	types::__id_t btn_id = this->btn_index.size();
	this->btn_index.resize(this->btn_index.size() + 1);

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
	this->btn_index(btn_id) = btn;
	return btn_id;
}

mdl::uint_t mdl::firefly::gui::btn_manager::create_btn(types::id_t __asset_id, types::_2d_coords_t __coords, uint_t __xaxis_len, uint_t __yaxis_len, asset_manager *__asset_manager) {
	types::__id_t btn_id = this->create_btn(__asset_manager->get_asset_data(__asset_id), __coords, __xaxis_len, __yaxis_len);
	this->btn_index[btn_id].inde_tx_mem = true;
	return btn_id;
}

bool mdl::firefly::gui::btn_manager::poll_event(types::btn_event_t& __btn_event, uint_t& __event_type) {
	if (this->event_queue.empty()) return false;
	__btn_event = this->event_queue.front().first;
	__event_type = this->event_queue.front().second;
	this->event_queue.pop();
	return true;
}

mdl::firefly::types::err_t mdl::firefly::gui::btn_manager::manage(types::pixmap_t __pixbuff) {
	if (!this->btn_index.size()) return FFLY_SUCCESS;
	types::__id_t btn_id = 0;
	auto update = [&btn_id]() {
		btn_id ++;
	};
	types::err_t any_err;
	while (btn_id != this->btn_index.size()) {
		types::btn_t& btn = this->btn_index[btn_id];
		if (!btn.enabled || (!__pixbuff && !this->pixbuff)) {
			update();
			continue;
		}

		if ((any_err = graphics::draw_pixmap(0, 0, btn.pixmap, btn.xaxis_len, btn.yaxis_len, btn.texture, btn.xaxis_len, btn.yaxis_len)) != FFLY_SUCCESS) return any_err;

		if (btn.draw_text_bm && btn.text_bm) {
			if ((any_err = graphics::draw_bitmap(btn.text_bm, btn.xaxis_len, btn.yaxis_len, btn.pixmap, btn.xaxis_len, btn.yaxis_len, btn.text_coords, btn.text_colour)) != FFLY_SUCCESS) return any_err;
		}

		if ((any_err = graphics::draw_pixmap(btn.coords.xaxis, btn.coords.yaxis, __pixbuff? __pixbuff : this->pixbuff, this->pb_xaxis_len, this->pb_yaxis_len, btn.pixmap, btn.xaxis_len, btn.yaxis_len)) != FFLY_SUCCESS) return any_err;
		if (btn.hover_enabled) {
			if (this->mouse_inside(btn.coords, btn.xaxis_len, btn.yaxis_len)) {
				if (btn.hover_fptr) btn.hover_fptr(btn_id, btn.voidptr);
				btn.mouse_hovering = true;
			} else
				btn.mouse_hovering = false;
		}

		if (btn.press_enabled) {
			if (btn.mouse_hovering) {
				if (*this->mouse_pressed) {
					if (!btn.already_pressed)
						if (btn.press_fptr) btn.press_fptr(btn_id, *this->mouse_btn_id, btn.voidptr);
					btn.already_pressed = true;
				} else
					btn.already_pressed = false;
			}
		}

		update();
	}

	return FFLY_SUCCESS;
}
