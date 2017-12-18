# include "room.hpp"
# include "system/errno.h"
# include "graphics/draw.h"
# include "memory/alloc_pixelmap.h"
# include "system/err.h"
# include "system/io.h"
mdl::firefly::types::err_t mdl::firefly::room::init(u16_t __width, u16_t __height) {
	if ((this->pixelmap = alloc_pixelmap(static_cast<uint_t>(__width), static_cast<uint_t>(__height))) == NULL) {
		system::io::fprintf(ffly_err, "failed to allocate pixelmap for room.\n");
		return FFLY_FAILURE;
	}
	this->width = __width;
	this->height = __height;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::room::draw(types::pixelmap_t __pixelbuff, u16_t __width, u16_t __height) {
	types::err_t err;
	if (_err(err = graphics::pixeldraw(0, 0, __pixelbuff, __width, this->pixelmap, this->width, this->height))) {
		system::io::fprintf(ffly_err, "failed to draw room.\n");
		return err;
	}
	return FFLY_SUCCESS;
}
