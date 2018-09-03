# include "font.hpp"

mdl::firefly::types::err_t mdl::firefly::font::load_font(boost::uint8_t *__data, uint_t __bytes) {
	if (FT_New_Memory_Face(this-> library, reinterpret_cast<FT_Byte *>(__data), __bytes, 0, &this-> face))
		return FFLY_FAILURE;

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::font::load_font(types::id_t __asset_id, asset_manager *__asset_manager) {
	asset_manager::asset_t asset = __asset_manager-> get_asset(__asset_id);
	if (this-> load_font(asset.data, asset.bytes) == FFLY_FAILURE)
		return FFLY_FAILURE;

	return FFLY_SUCCESS;
}
