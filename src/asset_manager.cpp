# include "asset_manager.hpp"

mdl::uint_t *mdl::firefly::asset_manager::load_asset(char const *__asset_file, uint_t __asset_type) {
	uint_t *asset_id;
	if (__asset_type == asset_manager::AST_PNG_FILE) {
		types::dsize_t *dsize = (types::dsize_t *)memory::mem_alloc(sizeof(types::dsize_t));
		types::pixmap_t pixmap;

		if (graphics::load_png_file("", __asset_file, pixmap, *dsize) != FFLY_SUCCESS) {
			return 0;
		}

		asset_id = this-> add_asset(pixmap, __asset_type);

		this-> asset_info(asset_id) = dsize;
	}

	return asset_id;
}

mdl::uint_t *mdl::firefly::asset_manager::add_asset(boost::uint8_t *__data, uint_t __type) {
	uint_t *asset_id = (uint_t *)memory::mem_alloc(sizeof(uint_t));
	this-> asset_indx.resize(this-> asset_indx.size() + 1);

	*asset_id = this-> asset_indx.size() - 1;
	this-> asset_indx[*asset_id].id = asset_id;
	this-> asset_indx[*asset_id].data = __data;
	this-> asset_indx[*asset_id].type = __type;

	printf("adding asset, id: %d\n", *asset_id);

	this-> asset_ids.insert(asset_id);
	return asset_id;
}

boost::int8_t mdl::firefly::asset_manager::de_init() {
	for (std::size_t o = 0; o != this-> asset_indx.size(); o ++) {
		printf("%s - asset_manager: freed memory for asset with id: %d\n", system::time_stamp(), o);
		if (this-> asset_indx[o].id != nullptr)
			memory::mem_free(this-> asset_indx[o].id);
		if (this-> asset_indx[o].info != nullptr)
			memory::mem_free(this-> asset_indx[o].info);
		if (this-> asset_indx[o].data != nullptr)
			memory::mem_free(this-> asset_indx[o].data);
	}
}

void mdl::firefly::asset_manager::del_asset(uint_t *__asset_id) {
	std::set<uint_t *>::iterator itor = this-> asset_ids.find(__asset_id);
	if (itor == this-> asset_ids.end()) {
		printf("failed to look up asset, id? %d\n", *__asset_id);
		return;
	}

	printf("deleting asset, id: %d\n", *__asset_id);

	this-> asset_indx(*__asset_id) = *(this-> asset_indx.end() - 1);
	*this-> asset_indx(*__asset_id).id = *__asset_id;

	memory::mem_free(this-> asset_indx[*__asset_id].id);
	memory::mem_free(this-> asset_indx[*__asset_id].data);

	this-> asset_indx.resize(this-> asset_indx.size() - 1);
	this-> asset_ids.erase(itor);
}

bool mdl::firefly::asset_manager::valid_asset_id(uint_t *__asset_id) {
	return this-> asset_ids.find(__asset_id) == this-> asset_ids.end()? false : true;
}

boost::uint8_t* mdl::firefly::asset_manager::get_asset_data(uint_t *__asset_id) {
	if (!this-> valid_asset_id(__asset_id)) return nullptr;
	return this-> asset_indx[*__asset_id].data;
}

mdl::firefly::asset_manager::asset_t mdl::firefly::asset_manager::get_asset(uint_t *__asset_id) noexcept {
	if (!this-> valid_asset_id(__asset_id)) throw;
	return this-> asset_indx[*__asset_id];
}

mdl::firefly::asset_manager::asset_t& mdl::firefly::asset_manager::asset(uint_t *__asset_id) {
	return this-> asset_indx[*__asset_id];
}

void * mdl::firefly::asset_manager::get_asset_info(uint_t *__asset_id) {
	return this-> asset_indx[*__asset_id].info;
}

void*& mdl::firefly::asset_manager::asset_info(uint_t *__asset_id) {
	return this-> asset_indx[*__asset_id].info;
}

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
