# ifndef __layer__hpp
# define __layer__hpp
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
# include <eint_t.hpp>
# include "graphics/draw_pixmap.hpp"
# include <utility>
namespace mdl {
namespace firefly {
class layer {
	public:
	typedef struct {
		uint_t xaxis_len, yaxis_len, xoffset, yoffset;

	} layer_info_t;

	uint_t add_layer(uint_t __xlen, uint_t __ylen, uint_t __xoffset, uint_t __yoffset, boost::uint8_t *__pixmap = nullptr) {
		uint_t layer_id = layers.size();
		layers.resize(layers.size() + 1);

		layer_info_t layer_info = {
			.xaxis_len = __xlen,
			.yaxis_len = __ylen,
			.xoffset = __xoffset,
			.yoffset = __yoffset
		};

		layers[layer_id].first = layer_info;

		if (__pixmap != nullptr) {
			layers[layer_id].second = __pixmap;
		} else {
			layers[layer_id].second = static_cast<boost::uint8_t *>(malloc(((__xlen * __ylen) * 4) * sizeof(boost::uint8_t)));
			memset(layers[layer_id].second, 0x0, ((__xlen * __ylen) * 4 * sizeof(boost::uint8_t)));
		}

		return layer_id;
	}

	boost::uint8_t* get_layer_pixmap(uint_t __layer_id) {
		return this-> layers[__layer_id].second;
	}

	void draw_layers(boost::uint8_t *__pixbuff, uint_t __xlen, uint_t __ylen) {
		for (std::size_t o = 0; o != layers.size(); o ++) {
			layer_info_t layer_info = this-> layers[o].first;
			boost::uint8_t *pixmap = this-> layers[o].second;

			graphics::draw_pixmap(layer_info.xoffset, layer_info.yoffset, __pixbuff, __xlen, __ylen, pixmap, layer_info.xaxis_len, layer_info.yaxis_len);

		}
	}

	~layer() {
		for (std::size_t o = 0; o != layers.size(); o ++)
			std::free(layers[o].second);
	}

	ublas::vector<std::pair<layer_info_t, boost::uint8_t *>> layers;
};
}
}

# endif /*__layer__hpp*/
