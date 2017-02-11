# include "client_gui.hpp"
mdl::client_gui::client_gui(boost::uint8_t *__wpixmap, mouse_coords_t *__mouse_coords, bool *__button_press, int *__button_code, uint_t __window_xlen, uint_t __window_ylen)
: wpixmap(__wpixmap), mouse_coords(__mouse_coords), button_press(__button_press), button_code(__button_code), window_xlen(__window_xlen), window_ylen(__window_ylen) {

}

bool mdl::client_gui::is_mouse_inside(uint_t __xaxis, uint_t __yaxis, uint_t __xlen, uint_t __ylen) {
	if ((this-> mouse_coords-> w_xaxis > __xaxis && this-> mouse_coords-> w_xaxis < (__xaxis + __xlen)) && (this-> mouse_coords-> w_yaxis > __yaxis && this-> mouse_coords-> w_yaxis < (__yaxis + __ylen))) return true;

	return false;
}

void mdl::client_gui::draw_buttons() {
	for (std::size_t o = 0; o != this-> button_index.size(); o ++) {
		int unsigned pixmap_size[2] = {this-> button_index[o].size[0], this-> button_index[o].size[1]};
		int unsigned a[2] = {(this-> window_xlen * this-> window_ylen) * 4, (pixmap_size[0] * pixmap_size[1]) * 4};
		int unsigned b[2] = {pixmap_size[0], pixmap_size[1]};

		bool is_mouse_overhead = this-> is_mouse_inside(this-> button_index[o].coords[0], this-> button_index[o].coords[1], pixmap_size[0], pixmap_size[1]);

		if (is_mouse_overhead && *this-> button_press) {
			if (this-> button_index[o].press_action != nullptr)
				this-> button_index[o].press_action(*this-> button_code);

			this-> button_index[o].already_pressed = true;
		} else this-> button_index[o].already_pressed = false;

		if (this-> button_index[o].do_hover_action && is_mouse_overhead) {
			if (this-> button_index[o].pixmap_id != 1 && this-> button_index[o].already_drawn)
				this-> button_index[o].already_drawn = false;

			this-> button_index[o].pixmap_id = 1;

			if (! this-> button_index[o].already_drawn) {
				this-> button_index[o].hover_action();
				draw_pixmap(this-> button_index[o].coords[0], this-> button_index[o].coords[1], this-> wpixmap, this-> button_index[o].hpixmap, a, b, this-> window_xlen);
				this-> button_index[o].already_drawn = true;
			}
		} else {
			if (this-> button_index[o].pixmap_id != 0 && this-> button_index[o].already_drawn)
				this-> button_index[o].already_drawn = false;

			this-> button_index[o].pixmap_id = 0;

			if (! this-> button_index[o].already_drawn) {
				draw_pixmap(this-> button_index[o].coords[0], this-> button_index[o].coords[1], this-> wpixmap, this-> button_index[o].pixmap, a, b, this-> window_xlen);
				this-> button_index[o].already_drawn = true;
			}
		}
	}
}

boost::uint8_t mdl::client_gui::create_button(boost::uint8_t *__pixmap, boost::uint8_t *__hpixmap, void (*__hover_action)(), void (* __press_action)(int), uint_t __xaxis, uint_t __yaxis, uint_t __xlen, uint_t __ylen) {
	std::size_t button_id = this-> button_index.size();
	this-> button_index.resize(this-> button_index.size() + 1);

	this-> button_index[button_id].coords[0] = __xaxis;
	this-> button_index[button_id].coords[1] = __yaxis;

	this-> button_index[button_id].size[0] = __xlen;
	this-> button_index[button_id].size[1] = __ylen;
	this-> button_index[button_id].pixmap = __pixmap;
	this-> button_index[button_id].press_action = __press_action;
	this-> button_index[button_id].hover_action = __hover_action;
	if (__hpixmap != nullptr) this-> button_index[button_id].do_hover_action = true;
	this-> button_index[button_id].hpixmap = __hpixmap;
}
