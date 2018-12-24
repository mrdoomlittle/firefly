# include "layer_manager.hpp"
# include "memory/alloc_pixelmap.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/errno.h"
# include "graphics/draw.h"
# include "system/err.h"
mdl::uint_t mdl::firefly::layer_manager::add_layer(u16_t __width, u16_t __height, u16_t __xa, u16_t __ya, types::pixelmap_t __pixelmap, types::err_t& __err){
	uint_t no = this->layers.size();
	layer _layer = {
		width:__width,
		height:__height,
		xa:__xa,
		ya:__ya
	};

	if(__pixelmap != nullptr)
		_layer.pixelmap = __pixelmap;
	else {
		if((_layer.pixelmap = memory::alloc_pixelmap(static_cast<uint_t>(__width), static_cast<uint_t>(__height))) == nullptr){
			system::io::fprintf(ffly_err, "layer_manager, failed to alloc memory for pixelmap.\n");
			__err = FFLY_FAILURE;
			return 0;
		}
	}

	system::io::fprintf(ffly_log, "layer, width: %u, height: %u, %p\n", _layer.width, _layer.height, _layer.pixelmap);
	this->layers.push_back(_layer);
	__err = FFLY_SUCCESS;
	return no;
}

mdl::firefly::types::err_t mdl::firefly::layer_manager::draw_layers(types::pixelmap_t __pixelbuff, u16_t __width, u16_t __height) {
	if (this->layers.empty()) {
		return FFLY_SUCCESS;
	}
	types::err_t err;
	layer *itr = this->layers.begin();
	while(itr <= this->layers.end()) {
		system::io::fprintf(ffly_log, "layer, width: %u, height: %u, %p\n", itr->width, itr->height, itr->pixelmap);
		if (_err(err = graphics::pixeldraw(itr->xa, itr->ya, __pixelbuff, __width, itr->pixelmap, itr->width, itr->height))) {
			system::io::fprintf(ffly_err, "failed to draw layer.\n");
			return err;
		}
		itr++;
	}
	return FFLY_SUCCESS;
}

/*
mdl::i8_tmdl::firefly::layer_manager::push_forwards(uint_t__layer_no){
	if(this->locked_layer(__layer_no))returnFFLY_NOP;

	this->render_arrm[__layer_no]=this->render_arrm[__layer_no+1];
	this->render_arrm[__layer_no+1]=__layer_no;
}

mdl::i8_tmdl::firefly::layer_manager::pull_backwards(uint_t__layer_no){
	if(this->locked_layer(__layer_no))returnFFLY_NOP;

	this->render_arrm[__layer_no]=this->render_arrm[__layer_no-1];
	this->render_arrm[__layer_no-1]=__layer_no;
}

mdl::i8_tmdl::firefly::layer_manager::swap(uint_t__layer_noa,uint_t__layer_nob){
	if(this->locked_layer(__layer_noa))returnFFLY_NOP;
	if(this->locked_layer(__layer_nob))returnFFLY_NOP;

	this->render_arrm[__layer_noa]=__layer_nob;
	this->render_arrm[__layer_nob]=__layer_noa;
}
*/
