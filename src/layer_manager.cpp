# include "layer_manager.hpp"
# include "graphics/draw_pixelmap.hpp"
# include "memory/alloc_pixelmap.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/errno.h"
mdl::uint_t mdl::firefly::layer_manager::add_layer(uint_t __xa_len,uint_t __ya_len,uint_t __xfs,uint_t __yfs,types::pixelmap_t __pixelmap, types::err_t& __any_err){
	uint_t no = this->layers.size();
	layer_t layer = {
		.desc = {
			xa_len:__xa_len,
			ya_len:__ya_len,
			xfs:__xfs,
			yfs:__yfs
		}
	};

	if(__pixelmap != nullptr)
		layer.pixelmap = __pixelmap;
	else {
		if((layer.pixelmap = memory::alloc_pixelmap(__xa_len, __ya_len)) == nullptr){
			system::io::printf(stderr,"layer_manager, failed to alloc memory for pixelmap.\n");
			__any_err = FFLY_FAILURE;
			return 0;
		}
	}

	this->layers.push_back(layer);
	__any_err = FFLY_SUCCESS;
	return no;
}

mdl::firefly::types::err_t mdl::firefly::layer_manager::draw_layers(types::pixelmap_t __pixelbuff, uint_t __xa_len, uint_t __ya_len) {
	layer_t *itr = this->layers.begin();
	while(itr <= this->layers.end()) {
 		graphics::gpu_draw_pixelmap(itr->desc.xfs, itr->desc.yfs, __pixelbuff, __xa_len, __ya_len, itr->pixelmap, itr->desc.xa_len, itr->desc.ya_len);
		itr++;
	}
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
