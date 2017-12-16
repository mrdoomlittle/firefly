# include "room.hpp"
# include "system/errno.h"
# include "graphics/draw.h"
# include "memory/alloc_pixelmap.h"
mdl::firefly::types::err_t mdl::firefly::room::init(u16_t __width, u16_t __height) {
	if ((this->pixelmap = alloc_pixelmap(static_cast<uint_t>(__width), static_cast<uint_t>(__height))) == NULL) {
		
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::room::draw(types::pixelmap_t __pixelbuff, u16_t __width, u16_t __height) {


	return FFLY_SUCCESS;
}
