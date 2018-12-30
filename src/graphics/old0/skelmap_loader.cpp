# include "skelmap_loader.hpp"
mdl::u8_t* mdl::firefly::graphics::load_skemap_file(types::skelmap_info_t& skelmap_info, char const *__file) {
	FILE *skelmap_file = fopen(__file, "rb");
	if (!skelmap_file) return nullptr;

	fread(&skelmap_info.xaxis_len, sizeof(mdl::uint_t), 1, skelmap_file);
	fread(&skelmap_info.yaxis_len, sizeof(mdl::uint_t), 1, skelmap_file);

	uint_t skelmap_size = skelmap_info.xaxis_len * skelmap_info.yaxis_len;
	mdl::u8_t *skelmap = static_cast<u8_t*>(malloc(skelmap_size*sizeof(u8_t)));

	bzero(skelmap, skelmap_size*sizeof(u8_t));

	fread(skelmap, 1, skelmap_size*sizeof(u8_t), skelmap_file);
	return skelmap;
}
