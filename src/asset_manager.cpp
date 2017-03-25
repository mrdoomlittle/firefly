# include "asset_manager.hpp"

mdl::uint_t *mdl::firefly::asset_manager::load_asset(char const *__asset_file, uint_t __asset_type) {
	if (__asset_type == AST_SKELMAP_T) {
/*
		types::skelmap_info_t skelmap_info;
		boost::uint8_t *skelmap = mdl::firefly::graphics::load_skemap_file(skelmap_info, __asset_file);

		this-> asset_data(this-> amount_of_assets) = skelmap;

		this-> asset_info[this-> amount_of_assets].asset_id = asset_id;
		this-> asset_info[this-> amount_of_assets].asset_type = SKELMAP_T;

		this-> asset_info[this-> amount_of_assets].skelmap_info = skelmap_info;

		this-> amount_of_assets ++;
		printf("added asset w: %d, file: %s\n", skelmap_info.xaxis_len, __asset_file);
*/
		//asset_id = this-> asset_info[(this-> amount_of_assets - 1)].asset_id;
	} else if (__asset_type == AST_PIXMAP_T) {



	}

	//uint_t *asset_id = this-> add_asset();

	//return asset_id;
}

mdl::uint_t *mdl::firefly::asset_manager::add_asset(boost::uint8_t *__data, uint_t __type) {
	uint_t *asset_id = (uint_t *)malloc(sizeof(uint_t));
	this-> asset_data.resize(this-> asset_data.size() + 1);
	this-> asset_info.resize(this-> asset_info.size() + 1);

	*asset_id = this-> asset_data.size() - 1;
	this-> asset_info[*asset_id].asset_id = asset_id;
	this-> asset_data[*asset_id] = __data;

	printf("adding asset, id: %d\n", *asset_id);

	this-> asset_ids.insert(asset_id);
	return asset_id;
}

void mdl::firefly::asset_manager::del_asset(uint_t *__asset_id) {
	std::set<uint_t *>::iterator itor = this-> asset_ids.find(__asset_id);
	if (itor == this-> asset_ids.end()) {
		printf("failed to look up asset, id? %d\n", *__asset_id);
		return;
	}

	printf("deleting asset, id: %d\n", *__asset_id);

	this-> asset_data(*__asset_id) = *(this-> asset_data.end() - 1);
	this-> asset_info(*__asset_id) = *(this-> asset_info.end() - 1);

	*this-> asset_info(*__asset_id).asset_id = *__asset_id;

	std::free(this-> asset_data(*__asset_id));

	this-> asset_data.resize(this-> asset_data.size() - 1);
	this-> asset_info.resize(this-> asset_info.size() - 1);

	this-> asset_ids.erase(itor);
}

bool mdl::firefly::asset_manager::valid_asset_id(uint_t *__asset_id) {
	return this-> asset_ids.find(__asset_id) == this-> asset_ids.end()? false : true;
}

boost::uint8_t* mdl::firefly::asset_manager::get_asset_data(uint_t *__asset_id) {
	if (!this-> valid_asset_id(__asset_id)) return nullptr;
	return this-> asset_data[*__asset_id];
}

static mdl::firefly::asset_manager::asset_info_t null_asset_info;
mdl::firefly::asset_manager::asset_info_t mdl::firefly::asset_manager::get_asset_info(uint_t *__asset_id) {
	if (!this-> valid_asset_id(__asset_id)) return null_asset_info;
	return this-> asset_info[*__asset_id];
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
