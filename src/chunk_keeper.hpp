# ifndef __chunk__keeper__hpp
# define __chunk__keeper__hpp
# include "ffly_config.hpp"
# include <eint_t.hpp>
# include <boost/cstdint.hpp>
# include <cstdio>
# include <set>
# include <errno.h>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/alloc_pixmap.hpp"
# include "system/errno.h"
# include "types/pixmap_t.h"
# include "types/uni_par_t.hpp"
# include "types/chunk_info_t.hpp"
# include "types/chunk_data_t.hpp"
namespace mdl {
namespace firefly {
class chunk_keeper {
	public:

	chunk_keeper(uint_t __xaxis_len, uint_t __yaxis_len, uint_t __zaxis_len)
	: xaxis_len(__xaxis_len), yaxis_len(__yaxis_len), zaxis_len(__zaxis_len) {}

	boost::int8_t create_chunk(uint_t*& __chunk_id, bool __overwrite = false);
	boost::int8_t del_chunk(uint_t *__chunk_id, bool __hard = false);

	boost::int8_t de_init() {
		for (uint_t chunk_id = 0; chunk_id != this-> chunk_count; chunk_id ++) {
			if (this-> unused_ids.find(this-> chunk_info[chunk_id].id) != this-> unused_ids.end()) {
				memory::mem_free(this-> chunk_info[chunk_id].id);
				continue;
			}

			types::chunk_info_t& chunk_info = this-> chunk_info[chunk_id];
			types::chunk_data_t& chunk_data = this-> chunk_data[chunk_id];

			memory::mem_free(chunk_info.id);
			memory::mem_free(chunk_data.particles);
			memory::mem_free(chunk_data.pixmap);
		}
	}

	types::chunk_info_t& get_chunk_info(uint_t *__chunk_id) {
		return this-> chunk_info[*__chunk_id];
	}

	types::chunk_data_t& get_chunk_data(uint_t *__chunk_id) {
		return this-> chunk_data[*__chunk_id];
	}

	uint_t get_chunk_xlen() { return this-> xaxis_len; }
	uint_t get_chunk_ylen() { return this-> yaxis_len; }
	uint_t get_chunk_zlen() { return this-> zaxis_len; }

	private:
	std::set<uint_t *> unused_ids;

	uint_t chunk_count = 0;
	types::chunk_info_t *chunk_info = nullptr;
	types::chunk_data_t *chunk_data = nullptr;
	uint_t const xaxis_len, yaxis_len, zaxis_len;
};
}
}

# endif /*__chunk__keeper__hpp*/
