# include "window.hpp"
boost::int8_t mdl::firefly::gui::window::init(boost::uint16_t __xaxis, boost::uint16_t __yaxis, boost::uint16_t __xaxis_len, boost::uint16_t __yaxis_len) {
	if ((this-> pixmap = memory::alloc_pixmap(__xaxis_len + (OUTLINE_WIDTH * 2), __yaxis_len + (OUTLINE_WIDTH * 2), 1)) == NULL) { 
		return FFLY_FAILURE;
	}

	this-> wpixmap = (types::pixmap_t *)memory::mem_alloc(((__xaxis_len * __yaxis_len) * 4) * sizeof(types::pixmap_t));
	for (uint_t y = 0; y != __yaxis_len; y ++) {
		for (uint_t x = 0; x != __xaxis_len; x ++) {
			uint_t point = ((x + OUTLINE_WIDTH) + ((y + OUTLINE_WIDTH) * (__xaxis_len + (OUTLINE_WIDTH * 2)))) * 4;
			this-> wpixmap[((x + (y * __xaxis_len)) * 4)] = (this-> pixmap + point);
			this-> wpixmap[((x + (y * __xaxis_len)) * 4) + 1] = (this-> pixmap + point + 1);
			this-> wpixmap[((x + (y * __xaxis_len)) * 4) + 2] = (this-> pixmap + point + 2);
			this-> wpixmap[((x + (y * __xaxis_len)) * 4) + 3] = (this-> pixmap + point + 3);
		}
	}

	this-> _wd_frame.set_pixmap(this-> pixmap);
	this-> _wd_frame.ex_fptr = &window::exit_btn;
	this-> _wd_frame.ex_arg = this;
	this-> _wd_frame.init(__xaxis, __yaxis, __xaxis_len, __yaxis_len, NULL);

	this-> pm_size.xaxis_len = __xaxis_len + (OUTLINE_WIDTH * 2);
	this-> pm_size.yaxis_len = __yaxis_len + (OUTLINE_WIDTH * 2);
	this-> coords.xaxis = __xaxis;
	this-> coords.yaxis = __yaxis;
	this-> wd_coords.xaxis = __xaxis + OUTLINE_WIDTH;
	this-> wd_coords.yaxis = __yaxis + OUTLINE_WIDTH;

	this-> wd_size.xaxis_len = __xaxis_len;
	this-> wd_size.yaxis_len = __yaxis_len;
}

boost::int8_t mdl::firefly::gui::window::draw(types::pixmap_t __pixbuff, types::dsize_t __pb_size) {
	if (this-> is_closed) return FFLY_SUCCESS;
	graphics::draw_pixmap(this-> coords.xaxis, this-> coords.yaxis, __pixbuff, __pb_size.xaxis_len, __pb_size.yaxis_len, this-> pixmap, this-> pm_size.xaxis_len, this-> pm_size.yaxis_len);
}

void mdl::firefly::gui::window::exit_btn(void *__arg) {
	window *_this = (window *)__arg;
	_this-> is_closed = true;
	_this-> de_init();
}

boost::int8_t mdl::firefly::gui::window::handle() {
	if (this-> is_closed) return FFLY_SUCCESS;
	this-> _wd_frame.handle();
	types::coords_t<boost::uint16_t> wdf_coords = this-> _wd_frame.get_coords();
	this-> coords.xaxis = wdf_coords.xaxis;
	this-> coords.yaxis = wdf_coords.yaxis;
	this-> wd_coords.xaxis = this-> coords.xaxis + OUTLINE_WIDTH;
	this-> wd_coords.yaxis = this-> coords.yaxis + OUTLINE_WIDTH;
	this-> wd_coords.zaxis = 0;
	if (maths::is_inside((types::coords_t<uint_t>)(*this-> mouse_coords), (types::coords_t<uint_t>)this-> wd_coords, this-> wd_size.xaxis_len, this-> wd_size.yaxis_len, 1)) {
		this-> pointer_coords.xaxis = this-> mouse_coords-> xaxis - this-> wd_coords.xaxis;
		this-> pointer_coords.yaxis = this-> mouse_coords-> yaxis - this-> wd_coords.yaxis;
	} else {
		this-> pointer_coords.xaxis = 0;
		this-> pointer_coords.yaxis = 0;
	}
}
