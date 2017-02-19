# ifndef __draw__text__hpp
# define __draw__text__hpp
# include "../types/skelfont_t.hpp"
# include "../asset_manager.hpp"
# include "../types/colour_t.hpp"
# include "../types/skelmap_info_t.hpp"
# include "draw_skelmap.hpp"
#include <unordered_map>
namespace mdl {
namespace firefly {
namespace graphics {
void draw_text(asset_manager *asset_man, boost::uint8_t *__pixbuff, types::skelfont_t *__skelfont, char const *__text, colour_t __colour, uint_t __xaxis, uint_t __yaxis, uint_t __scale, uint_t __pb_xlen, uint_t __pb_ylen, uint_t __spacing) {
	uint_t xaxis_offset = __xaxis;
	for (std::size_t i = 0; i != strlen(__text); i ++) {
//		for (std::size_t o = 0; o != __skelfont-> font_size; o ++) {
		if (__skelfont-> font_data.find(__text[i]) != __skelfont-> font_data.end()) {
	//		if (__skelfont-> font_data[o].first == __text[i]) {
				uint_t asset_id = __skelfont-> font_data[__text[i]];

				boost::uint8_t* skelmap = asset_man-> get_asset_data(asset_id);

				asset_manager::asset_info_t asset_info = asset_man-> get_asset_info(asset_id);

				types::skelmap_info_t skelmap_info = asset_info.skelmap_info;

				mdl::firefly::graphics::draw_skelmap(
					skelmap, __pixbuff, xaxis_offset, __yaxis, skelmap_info, __pb_xlen, __pb_ylen, __scale, __colour
				);

				xaxis_offset += skelmap_info.xaxis_len + __spacing;
	//		}
		}
	}
}
}
}
}

# endif /*__draw__text__hpp*/
