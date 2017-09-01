# ifndef __layer__hpp
# define __layer__hpp
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
# include <mdlint.h>
# include "graphics/draw_pixmap.hpp"
# include <utility>
# include <string.h>
# include <set>
# include "types/layer_info_t.hpp"
# include "memory/alloc_pixmap.hpp"
# include "memory/mem_free.h"
# include "types/pixmap_t.h"
# include "system/io.h"
namespace mdl {
namespace firefly {
class layer_manager {
	public:
	uint_t add_layer(uint_t __xlen, uint_t __ylen, uint_t __xoffset, uint_t __yoffset, boost::uint8_t *__pixmap = nullptr) {
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

		layers[layer_id].first = layer_info;

		if (__pixmap != nullptr) {
			layers[layer_id].second = __pixmap;
		} else {
			layers[layer_id].second = memory::alloc_pixmap(__xlen, __ylen);
			if (layers[layer_id].second == NULL) {
				system::io::printf(stderr, "layer_manager: failed to alloc memory for pixel map.\n");
				return 0;
			}

			memset(layers[layer_id].second, 0x0, ((__xlen * __ylen) * 4 * sizeof(boost::uint8_t)));
		}

		return layer_id;
	}

	bool locked_layer(uint_t __layer_id) {
		if (this-> locked_layers.find(__layer_id) != this-> locked_layers.end()) return false;
		return true;
	}

	void lock_layer(uint_t __layer_id) {
		this-> locked_layers.insert(__layer_id);
	}

	void ulock_layer(uint_t __layer_id) {
		std::set<bool>::iterator itor = this-> locked_layers.find(__layer_id);
		if (itor == this-> locked_layers.end()) return;

		this-> locked_layers.erase(itor);
	}

	bool does_layer_exist(uint_t __layer_id) {
		return this-> layers.size() >= (__layer_id + 1)? true : false;
	}

	types::pixmap_t get_layer_pixmap(uint_t __layer_id) {
		return this-> layers[__layer_id].second;
	}

	types::layer_info_t get_layer_info(uint_t __layer_id) {
		return this-> layers[__layer_id].first;
	}

	// change to int8_t
	i8_t draw_layers(types::pixmap_t __pixbuff, uint_t __xlen, uint_t __ylen) {
		for (std::size_t o = 0; o != layers.size(); o ++) {
			uint_t layer_id = this-> render_arrm[o];
			types::layer_info_t layer_info = this-> layers[layer_id].first;
			types::pixmap_t pixmap = this-> layers[layer_id].second;

			if (pixmap == nullptr) continue;

			if (graphics::draw_pixmap(layer_info.xoffset, layer_info.yoffset, __pixbuff, __xlen, __ylen, pixmap, layer_info.xaxis_len, layer_info.yaxis_len) == FFLY_FAILURE) {
				system::io::printf(stderr, "layer_manager: failed to draw layer[gpu], using cpu.\n");
				return FFLY_FAILURE;
			}
		}
		return FFLY_SUCCESS;
	}


	i8_t push_forwards(uint_t __layer_id);
	i8_t pull_backwards(uint_t __layer_id);
	i8_t swap(uint_t __layer_ida, uint_t __layer_idb);
	~layer_manager() {this->de_init();}

	types::err_t de_init() {
		for (std::size_t layer_no{}; layer_no != layers.size(); layer_no++)
			memory::mem_free(layers[layer_no].second);
	}

	std::set<bool> locked_layers;
	ublas::vector<uint_t> render_arrm; // render arrangement
	ublas::vector<std::pair<types::layer_info_t, types::pixmap_t>> layers;
};
}
}

# endif /*__layer__hpp*/
