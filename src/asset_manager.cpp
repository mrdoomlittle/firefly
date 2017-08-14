# include "asset_manager.hpp"
mdl::firefly::types::id_t mdl::firefly::asset_manager::load_asset(char const *__asset_file, uint_t __asset_type) {
	types::id_t asset_id;
	if (__asset_type == asset_manager::AST_PNG_FILE) {
		types::_2d_dsize_t<> *dsize = (types::_2d_dsize_t<> *)memory::mem_alloc(sizeof(types::_2d_dsize_t<>));
		types::pixmap_t pixmap;

		if (graphics::load_png_file("", __asset_file, pixmap, *dsize) != FFLY_SUCCESS) {
			return nullptr;
		}

		asset_id = this->add_asset(pixmap, 0, __asset_type);

		this->asset_info(asset_id) = dsize;
	}

	return asset_id;
}

mdl::firefly::types::id_t mdl::firefly::asset_manager::add_asset(u8_t *__data, uint_t __bytes, uint_t __type) {
	types::id_t asset_id = (types::id_t)memory::mem_alloc(sizeof(types::__id_t));
	this->asset_indx.resize(this->asset_indx.size()+1);

	*asset_id = this->asset_indx.size()-1;
	this->asset_indx[*asset_id] = (asset_t)	{
		.id = asset_id,
		.type = __type,
		.bytes = __bytes,
		.data = __data,
		.info = nullptr
	};

	system::io::printf(stdout, "asset_manager: adding asset with id: %u\n", *asset_id);

	this->asset_ids.insert(asset_id);
	return asset_id;
}

mdl::firefly::types::err_t mdl::firefly::asset_manager::de_init() {
	for (std::size_t o = 0; o != this->asset_indx.size(); o ++) {
		printf("%s - asset_manager: freed memory for asset with id: %d\n", system::time_stamp(), o);
		if (this->asset_indx[o].id != nullptr)
			memory::mem_free(this->asset_indx[o].id);
		if (this->asset_indx[o].info != nullptr)
			memory::mem_free(this->asset_indx[o].info);
		if (this->asset_indx[o].data != nullptr)
			memory::mem_free(this->asset_indx[o].data);
	}
}

void mdl::firefly::asset_manager::del_asset(types::id_t __asset_id) {
	std::set<types::id_t>::iterator itr = this->asset_ids.find(__asset_id);
	if (itr == this->asset_ids.end()) {
		printf("failed to look up asset, id? %d\n", *__asset_id);
		return;
	}

	printf("deleting asset, id: %d\n", *__asset_id);

	this->asset_indx(*__asset_id) = *(this->asset_indx.end()-1);
	*this->asset_indx(*__asset_id).id = *__asset_id;

	memory::mem_free(this->asset_indx[*__asset_id].id);
	memory::mem_free(this->asset_indx[*__asset_id].data);

	this->asset_indx.resize(this->asset_indx.size()-1);
	this->asset_ids.erase(itr);
}

mdl::firefly::types::bool_t mdl::firefly::asset_manager::valid_asset_id(types::id_t __asset_id) {
	return this->asset_ids.find(__asset_id) == this->asset_ids.end()? false : true;}

mdl::u8_t* mdl::firefly::asset_manager::get_asset_data(types::id_t __asset_id) {
	if (!this->valid_asset_id(__asset_id)) return nullptr;
	return this->asset_indx[*__asset_id].data;
}

mdl::firefly::asset_manager::asset_t mdl::firefly::asset_manager::get_asset(types::id_t __asset_id) noexcept {
	if (!this->valid_asset_id(__asset_id)) throw;
	return this->asset_indx[*__asset_id];
}

mdl::firefly::asset_manager::asset_t& mdl::firefly::asset_manager::asset(types::id_t __asset_id) {
	return this->asset_indx[*__asset_id];}

void* mdl::firefly::asset_manager::get_asset_info(types::id_t __asset_id) {
	return this->asset_indx[*__asset_id].info;}

void*& mdl::firefly::asset_manager::asset_info(types::id_t __asset_id) {
	return this->asset_indx[*__asset_id].info;}
/*
int main() {
	mdl::firefly::asset_manager ast_manager;

	boost::uint8_t *asset_data = (boost::uint8_t *)malloc(8);
	mdl::uint_t *_iid;
	mdl::uint_t *asset_id;
	for (std::size_t o = 0; o != 12; o ++) {
		asset_id = ast_manager.add_asset(asset_data, AST_PIXMAP_T);
		if (o == 4) _iid = asset_id;
	}

	ast_manager.del_asset(_iid);
	printf("id: %d, %d\n", *_iid, *asset_id);
}*/
