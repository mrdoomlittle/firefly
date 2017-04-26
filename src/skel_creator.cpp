# include "skel_creator.hpp"
mdl::firefly::types::err_t mdl::firefly::skel_creator::init(uint_t __pm_xlen, uint_t __pm_ylen, uint_t __xslpit, uint_t __ysplit) {
	if ((this-> grid_pm = memory::alloc_pixmap(__pm_xlen, __pm_ylen)) == NULL) {
		fprintf(stderr, "skel_creator: failed to alloc memory for 'grid_pm', errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	firefly::graphics::draw_grid(this-> grid_pm, __pm_xlen, __pm_ylen, __xsplit, __ysplit, 4, 4, graphics::__colour__(64, 73, 89, 244));
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::skel_creator::init() {
	memory::mem_free(this-> grid_pm);
}

mdl::firefly::types::err_t mdl::firefly::skel_creator::tick() {

}

mdl::firefly::types::err_t mdl::firefly::skel_creator::draw(types::_1d_pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen) {

}
