# include "skel_creator.hpp"
# define GRID_LN_WIDTH 4
# define GRIP_LN_HEIGHT 4
mdl::firefly::types::err_t mdl::firefly::skel_creator::init(uint_t __pm_xlen, uint_t __pm_ylen, uint_t __xsplit, uint_t __ysplit) {
	if (__pm_xlen % __xsplit || __pm_ylen % __ysplit) {
		return FFLY_FAILURE;
	}

	if ((this-> bse_pm = memory::alloc_pixmap(__pm_xlen, __pm_ylen)) == NULL) {
		fprintf(stderr, "skel_creator: failed to alloc memory for 'bse_pm', errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	if ((this-> blk_st = (u8_t *)memory::mem_alloc((__xsplit * __ysplit) * sizeof(u8_t))) == NULL) {
		fprintf(stderr, "skel_creator: failed to alloc memory for 'blk_st', errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	memory::mem_init(this-> blk_st, (__xsplit * __ysplit) * sizeof(u8_t));

	this-> bpm_xa_len = __pm_xlen;
	this-> bpm_ya_len = __pm_ylen;
	this-> xsplit = __xsplit;
	this-> ysplit = __ysplit;

	firefly::graphics::draw_grid(this-> bse_pm, __pm_xlen, __pm_ylen, __xsplit, __ysplit, GRID_LN_WIDTH, GRIP_LN_HEIGHT, graphics::__colour__(64, 73, 89, 244));
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::skel_creator::de_init() {
	memory::mem_free(this-> bse_pm);
	memory::mem_free(this-> blk_st);
}

mdl::firefly::types::err_t mdl::firefly::skel_creator::tick(u16_t __mx_axis, u16_t __my_axis, bool __mpress, int __button_code) {
	bool static one_press = false;

	uint_t blk_xc = floor(__mx_axis/(this-> bpm_xa_len/this-> xsplit));
	uint_t blk_yc = floor(__my_axis/(this-> bpm_ya_len/this-> ysplit));
	uint_t pm_xa = blk_xc * (this-> bpm_xa_len/this-> xsplit);
	uint_t pm_ya = blk_yc * (this-> bpm_ya_len/this-> ysplit);
	printf("%d - %d - %d\n", __mpress, __button_code, one_press);
	if (__mpress && __button_code == 1 && !one_press) {
		printf("dhhfdhfhdu\n");
		graphics::colour_t colour;
		uint_t blk_point = blk_xc + (blk_yc * this-> xsplit);
		colour = !this-> blk_st[blk_point]? graphics::__colour__(0, 0, 0, 244) : graphics::__colour__(244, 244, 244, 244);
		this-> blk_st[blk_point] = !this-> blk_st[blk_point]? 0x1 : 0x0;

		if (graphics::draw_rect(this-> bse_pm, pm_xa + GRID_LN_WIDTH, pm_ya + GRIP_LN_HEIGHT, pm_xa + (this-> bpm_xa_len/this-> xsplit), pm_ya + (this-> bpm_ya_len/this-> ysplit), colour, this-> bpm_xa_len, this-> bpm_ya_len) != FFLY_SUCCESS) {
			return FFLY_FAILURE;
		}
		one_press = true;
	}

	if (!__mpress) one_press = false;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::skel_creator::draw(types::_1d_pm_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen) {
	firefly::graphics::draw_pixmap(0, 0, __pixbuff, __pb_xlen, __pb_ylen, this-> bse_pm, this-> bpm_xa_len, this-> bpm_ya_len);
}
