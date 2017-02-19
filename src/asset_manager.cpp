# include "asset_manager.hpp"

mdl::uint_t mdl::firefly::asset_manager::load_asset(char const *__asset_file, uint_t __asset_type) {
	if (__asset_type == SKELMAP_T) {
		types::skelmap_info_t skelmap_info;
		boost::uint8_t *skelmap = mdl::firefly::graphics::load_skemap_file(skelmap_info, __asset_file);

		this-> asset_data.resize(this-> amount_of_assets + 1);
		this-> asset_info.resize(this-> amount_of_assets + 1);

		this-> asset_data(this-> amount_of_assets) = skelmap;

		this-> asset_info[this-> amount_of_assets].asset_id = this-> amount_of_assets;
		this-> asset_info[this-> amount_of_assets].asset_type = SKELMAP_T;

		this-> asset_info[this-> amount_of_assets].skelmap_info = skelmap_info;

		this-> amount_of_assets ++;
		printf("added asset w: %d, file: %s\n", skelmap_info.xaxis_len, __asset_file);

		return this-> asset_info[(this-> amount_of_assets - 1)].asset_id;
	}

	return 0;
}

boost::uint8_t* mdl::firefly::asset_manager::get_asset_data(uint_t __asset_id) {
	return this-> asset_data[__asset_id];
}

mdl::firefly::asset_manager::asset_info_t mdl::firefly::asset_manager::get_asset_info(uint_t __asset_id) {
	return this-> asset_info[__asset_id];
}
