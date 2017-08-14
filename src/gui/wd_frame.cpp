# include "wd_frame.hpp"
static const mdl::firefly::graphics::colour_t ol_colour = {36, 37, 38, 244};
mdl::firefly::types::err_t mdl::firefly::gui::wd_frame::init(u16_t __xaxis, u16_t __yaxis, u16_t __xaxis_len, u16_t __yaxis_len, char const *__title) {
	if (!this->pixmap) {
		if ((this->pixmap = memory::alloc_pixmap(static_cast<uint_t>(__xaxis_len+(OUTLINE_WIDTH*2)), static_cast<uint_t>(__yaxis_len+(OUTLINE_WIDTH*2)))) == NULL) {
			system::io::printf(stderr, "wd_frame: failed to alloc memory for frame pixmap, errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	}

	this->pm_size.xaxis_len = __xaxis_len+(OUTLINE_WIDTH*2);
	this->pm_size.yaxis_len = __yaxis_len+(OUTLINE_WIDTH*2);

	if (!this->pixmap)
		memset(this->pixmap, 22, (this->pm_size.xaxis_len * this->pm_size.yaxis_len) * 4);

	types::err_t any_err;
	if ((any_err = graphics::draw_outline(this->pixmap, types::coords<uint_t>(__xaxis+OUTLINE_WIDTH, __yaxis+OUTLINE_WIDTH, 0),
		types::coords<uint_t>(__xaxis+__xaxis_len+OUTLINE_WIDTH, __yaxis+__yaxis_len+OUTLINE_WIDTH, 0), OUTLINE_WIDTH, this->pm_size, ol_colour)) != FFLY_SUCCESS) return any_err;

	this->coords.xaxis = __xaxis;
	this->coords.yaxis = __yaxis;
	this->coords.zaxis = 0;

	this->ex_btn_coords.xaxis = floor(OUTLINE_WIDTH/3);
	this->ex_btn_coords.yaxis = floor(OUTLINE_WIDTH/3);

	this->ex_btn_size.xaxis_len = OUTLINE_WIDTH-floor(OUTLINE_WIDTH/3)*2;
	this->ex_btn_size.yaxis_len = OUTLINE_WIDTH-floor(OUTLINE_WIDTH/3)*2;
	for (uint_t y = this->ex_btn_coords.yaxis; y != OUTLINE_WIDTH-floor(OUTLINE_WIDTH/3); y++) {
		for (uint_t x = this->ex_btn_coords.xaxis; x != OUTLINE_WIDTH-floor(OUTLINE_WIDTH/3); x++) {
			uint_t point = (x+(y * this->pm_size.xaxis_len)) * 4;
			this->pixmap[point] = 168;
			this->pixmap[point+1] = 70;
			this->pixmap[point+2] = 69;
			this->pixmap[point+3] = 244;
		}
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::gui::wd_frame::draw(types::pixmap_t __pixbuff, types::dsize_t __pb_size) {
	types::err_t any_err;
	if ((any_err = graphics::draw_pixmap(this->coords.xaxis, this->coords.yaxis, __pixbuff, __pb_size.xaxis_len, __pb_size.yaxis_len, this->pixmap, this->pm_size.xaxis_len, this->pm_size.yaxis_len)) != FFLY_SUCCESS)
		return any_err;
	this->pb_size = __pb_size;
	return FFLY_SUCCESS;
}

bool mdl::firefly::gui::wd_frame::is_inside() {
	types::coords_t<uint_t> pointer_coords = types::make_coords<uint_t>(core_portal.pointer_coords->wd.xaxis, core_portal.pointer_coords->wd.yaxis, 0);

	if (maths::is_inside(pointer_coords, this->coords, this->pm_size.xaxis_len, OUTLINE_WIDTH, 1)) return true;
	if (maths::is_inside(pointer_coords, this->coords, OUTLINE_WIDTH, this->pm_size.yaxis_len, 1)) return true;
	if (maths::is_inside(pointer_coords, types::coords<uint_t>((this->coords.xaxis+this->pm_size.xaxis_len)-OUTLINE_WIDTH, this->coords.yaxis, 0), OUTLINE_WIDTH, this->pm_size.yaxis_len, 1)) return true;
	if (maths::is_inside(pointer_coords, types::coords<uint_t>(this->coords.xaxis, (this->coords.yaxis+this->pm_size.yaxis_len)-OUTLINE_WIDTH, 0), this->pm_size.xaxis_len, OUTLINE_WIDTH, 1)) return true;
	return false;
}

mdl::firefly::types::err_t mdl::firefly::gui::wd_frame::handle() {
	static bool frame_clock = false;
	static uint_t xaxis_dist = 0, yaxis_dist = 0;
	if (this->is_inside() && *this->mouse_press && *this->mouse_btn_id == 1) {
		if (!frame_clock) {
			xaxis_dist = core_portal.pointer_coords->wd.xaxis-this->coords.xaxis;
        	yaxis_dist = core_portal.pointer_coords->wd.yaxis-this->coords.yaxis;
			frame_clock = true;
		}
	}

	if (!*this->mouse_press) frame_clock = false;

	if (frame_clock) {
		int_t n_xaxis = (int_t)core_portal.pointer_coords->wd.xaxis-(int_t)xaxis_dist;
		int_t n_yaxis = (int_t)core_portal.pointer_coords->wd.yaxis-(int_t)yaxis_dist;

//		if ((n_xaxis+this->pm_size.xaxis_len) >= this->pb_size.xaxis_len)
//			this->coords.xaxis = this->pb_size.xaxis_len-this->pm_size.xaxis_len-1;
//		else
		if (n_xaxis+this->pm_size.xaxis_len <= *core_portal.wd_xa_len)
			this->coords.xaxis = n_xaxis < 0? 0 : n_xaxis;
		else
			this->coords.xaxis = (*core_portal.wd_xa_len)-this->pm_size.xaxis_len;

//		if ((n_yaxis+this->pm_size.yaxis_len) >= this->pb_size.yaxis_len)
//			this->coords.yaxis = this->pb_size.yaxis_len-this->pm_size.yaxis_len-1;
//		else
		if (n_yaxis+this->pm_size.yaxis_len <= *core_portal.wd_ya_len)
			this->coords.yaxis = n_yaxis < 0? 0 : n_yaxis;
		else
			this->coords.yaxis = (*core_portal.wd_ya_len)-this->pm_size.yaxis_len;
	}

	types::coords_t<uint_t> pointer_coords = types::make_coords<uint_t>(core_portal.pointer_coords->wd.xaxis, core_portal.pointer_coords->wd.yaxis, 0);
	static bool ex_btn_pressed = false;
	if (maths::is_inside(pointer_coords, types::coords(this->coords.xaxis+this->ex_btn_coords.xaxis, this->coords.yaxis+this->ex_btn_coords.yaxis, 0), this->ex_btn_size.xaxis_len, this->ex_btn_size.yaxis_len, 1)) {
		if (*this->mouse_press && *this->mouse_btn_id == 1) {
			if (!ex_btn_pressed) {
				printf("exit btn pressed.\n");
				if (this->ex_fptr != nullptr)
					this->ex_fptr(this->ex_arg);
				ex_btn_pressed = true;
			}
		} else
			ex_btn_pressed = false;
	}
	return FFLY_SUCCESS;
}
