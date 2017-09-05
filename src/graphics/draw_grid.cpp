# include "draw_grid.hpp"
mdl::firefly::types::err_t mdl::firefly::graphics::cpu_draw_grid(types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, uint_t __xsplit, uint_t __ysplit, uint_t __horz_thickness, uint_t __vert_thickness, types::colour_t __line_colour) {
	if (__pm_xlen % __xsplit || __pm_ylen % __ysplit) {
		fprintf(stderr, "draw_grid: failure, pixmap xlen and ylen must split evenly.\n");
		return FFLY_FAILURE;
	}

	uint_t blk_xlen = __pm_xlen / __xsplit;
	uint_t blk_ylen = __pm_ylen / __ysplit;
	if (__horz_thickness > blk_xlen || __vert_thickness > blk_ylen) {
		fprintf(stderr, "draw_grid: failure, horz and vert thickness must be smaller then the block xlen and ylen.\n");
		return FFLY_FAILURE;
	}

	for (uint_t ya{}; ya != __pm_ylen; ya += blk_ylen) {
		for (uint_t xa{}; xa != __pm_xlen; xa += blk_xlen) {
			uint_t pm_blk_point = (xa + (ya * __pm_xlen)) * 4;

			// vert
			for (uint_t _ya{}; _ya != __vert_thickness; _ya ++) {
				for (uint_t _xa{}; _xa != blk_xlen; _xa ++) {
					uint_t pm_point = pm_blk_point + ((_xa + (_ya * __pm_xlen)) * 4);
					__pixmap[pm_point] = __line_colour.r;
					__pixmap[pm_point + 1] = __line_colour.g;
					__pixmap[pm_point + 2] = __line_colour.b;
					__pixmap[pm_point + 3] = __line_colour.a;
				}
			}

			// horz
			for (uint_t _ya{}; _ya != blk_ylen; _ya ++) {
				for (uint_t _xa{}; _xa != __horz_thickness; _xa ++) {
					uint_t pm_point = pm_blk_point + ((_xa + (_ya * __pm_xlen)) * 4);
					__pixmap[pm_point] = __line_colour.r;
					__pixmap[pm_point + 1] = __line_colour.g;
					__pixmap[pm_point + 2] = __line_colour.b;
					__pixmap[pm_point + 3] = __line_colour.a;
				}
			}
		}
	}
	return FFLY_SUCCESS;
}
