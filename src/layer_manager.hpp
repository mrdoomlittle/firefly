# ifndef __layer__hpp
# define __layer__hpp
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
# include <eint_t.hpp>
# include "graphics/draw_pixmap.hpp"
# include <utility>
# include <string.h>
# include "types/layer_info_t.hpp"
# include "memory/alloc_pixmap.hpp"
# include "memory/mem_free.h"
# include "types/pixmap_t.h"
namespace mdl {
namespace firefly {
class layer {
	public:
	uint_t add_layer(uint_t __xlen, uint_t __ylen, uint_t __xoffset, uint_t __yoffset){//, boost::uint8_t *__pixmap = nullptr) {
		uint_t layer_id = layers.size();
		layers.resize(layers.size() + 1);
		render_arrm.resize(render_arrm.size() + 1);


		render_arrm[layer_id] = layer_id;

		types::layer_info_t layer_info = {
			.xaxis_len = __xlen,
			.yaxis_len = __ylen,
			.xoffset = __xoffset,
			.yoffset = __yoffset
		};

		//printf("hello\n");
		layers[layer_id].first = layer_info;

//		if (__pixmap != nullptr) {
//			layers[layer_id].second = __pixmap;
//		} else {
			layers[layer_id].second = memory::alloc_pixmap(__xlen, __ylen, 1);
			//layers[layer_id].second = static_cast<boost::uint8_t *>(malloc(((__xlen * __ylen) * 4) * sizeof(boost::uint8_t)));
			memset(layers[layer_id].second, 0x0, ((__xlen * __ylen) * 4 * sizeof(boost::uint8_t)));
//		}

		return layer_id;
	}

	bool does_layer_exist(uint_t __layer_id) {
		return this-> layers.size() >= (__layer_id + 1)? true : false;
	}

	boost::uint8_t* get_layer_pixmap(uint_t __layer_id) {
		return this-> layers[__layer_id].second;
	}

	types::layer_info_t get_layer_info(uint_t __layer_id) {
		return this-> layers[__layer_id].first;
	}

	// change to int8_t
	boost::int8_t draw_layers(types::pixmap_t __pixbuff, uint_t __xlen, uint_t __ylen) {
		for (std::size_t o = 0; o != layers.size(); o ++) {
			uint_t layer_id = this-> render_arrm[o];
			types::layer_info_t layer_info = this-> layers[layer_id].first;
			types::pixmap_t pixmap = this-> layers[layer_id].second;

			if (graphics::draw_pixmap(layer_info.xoffset, layer_info.yoffset, __pixbuff, __xlen, __ylen, pixmap, layer_info.xaxis_len, layer_info.yaxis_len) == FFLY_FAILURE) {
				fprintf(stderr, "failed to draw a layer.\n");
				return FFLY_FAILURE;
			}
		}
		return FFLY_SUCCESS;
	}


	boost::int8_t push_forwards(uint_t __layer_id);
	boost::int8_t pull_backwards(uint_t __layer_id);
	boost::int8_t swap(uint_t __layer_ida, uint_t __layer_idb);
	~layer() {
		/* free alloced pixmap for each payer */
		for (std::size_t o = 0; o != layers.size(); o ++)
			memory::mem_free(layers[o].second);
	}

	ublas::vector<uint_t> render_arrm; // render arrangement
	ublas::vector<std::pair<types::layer_info_t, types::pixmap_t>> layers;
};
}
}

# endif /*__layer__hpp*/
