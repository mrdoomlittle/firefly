# ifndef __uni__manager__hpp
# define __uni__manager__hpp
# include "types/pixmap_t.h"
# include "types/uni_par_t.hpp"
# include <eint_t.hpp>
# include "ffly_config.hpp"
# include "chunk_manager.hpp"
# include "system/errno.h"
# include <errno.h>
# include <math.h>
# include <cstdio>
# include "types/coords_t.hpp"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
namespace mdl {
namespace firefly {
class uni_manager {
	public:
	uni_manager(uint_t __xaxis_len, uint_t __yaxis_len, uint_t __zaxis_len)
	: xaxis_len(__xaxis_len), yaxis_len(__yaxis_len), zaxis_len(__zaxis_len) {}

	boost::int8_t init(uint_t __xaxis_split, uint_t __yaxis_split, uint_t __zaxis_split);

	void get_chunk_coords(types::coords_t<> __coords, uint_t& __xaxis, uint_t& __yaxis, uint_t& __zaxis) {
		__xaxis = floor(__coords.xaxis / this-> _chunk_manager-> get_chunk_xlen());
		__yaxis = floor(__coords.yaxis / this-> _chunk_manager-> get_chunk_ylen());
		__zaxis = floor(__coords.zaxis / this-> _chunk_manager-> get_chunk_zlen());
	}

	boost::int8_t de_init() {
		this-> _chunk_manager-> de_init();
		memory::mem_free(this-> _chunk_manager);
		memory::mem_free(this-> chunk_id_index);
		memory::mem_free(this-> uni_pixmap);
		memory::mem_free(this-> uni_particles);
	}

	types::pixmap_t chunk_pixmap(types::coords_t<> __coords) {
		uint_t xchunk, ychunk, zchunk;
		this-> get_chunk_coords(__coords, xchunk, ychunk, zchunk);
		return this-> uni_pixmap[xchunk * ychunk * zchunk];
	}

	types::uni_par_t * chunk_particles(types::coords_t<> __coords) {
		uint_t xchunk, ychunk, zchunk;
		this-> get_chunk_coords(__coords, xchunk, ychunk, zchunk);
		return this-> uni_particles[xchunk * ychunk * zchunk];
	}

	boost::int8_t save();
	boost::int8_t load();

	private:
	chunk_manager *_chunk_manager;
	uint_t chunk_xcount, chunk_ycount, chunk_zcount;
	uint_t const xaxis_len, yaxis_len, zaxis_len;
	uint_t **chunk_id_index;
	types::pixmap_t *uni_pixmap = nullptr;
	types::uni_par_t **uni_particles = nullptr;
};
}
}

# endif /*__uni__manager__hpp*/
