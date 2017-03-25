# ifndef __chunk__manager__hpp
# define __chunk__manager__hpp
# include "chunk_keeper.hpp"
# include <eint_t.hpp>
# include <set>
# include "types/coords_t.hpp"
# include "types/chunk_info_t.hpp"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "maths/is_inside.hpp"
namespace mdl {
namespace firefly {
class chunk_manager {
	public:
	chunk_manager(uint_t __xaxis_len, uint_t __yaxis_len, uint_t __zaxis_len)
	: _chunk_keeper(__xaxis_len, __yaxis_len, __zaxis_len) {}

	boost::int8_t add_chunk(uint_t*& __chunk_id, uint_t __xaxis, uint_t __yaxis, uint_t __zaxis);
	boost::int8_t rm_chunk(uint_t __xaxis, uint_t __yaxis, uint_t __zaxis);

	uint_t* find_chunk_id(uint_t __xaxis, uint_t __yaxis, uint_t __zaxis) {
		for (std::set<uint_t *>::iterator itor = this-> chunk_index.begin(); itor != this-> chunk_index.end(); ++itor) {
			types::chunk_info_t& chunk_info = this-> _chunk_keeper.get_chunk_info(*itor);

			if (chunk_info.coords.xaxis == __xaxis && chunk_info.coords.yaxis == __yaxis && chunk_info.coords.zaxis == __zaxis) {
				return *itor;
			}
		}
	}

	uint_t* coords_to_id(uint_t __xaxis, uint_t __yaxis, uint_t __zaxis) {
		for (std::set<uint_t *>::iterator itor = this-> chunk_index.begin(); itor != this-> chunk_index.end(); ++itor) {
			types::chunk_info_t& chunk_info = this-> _chunk_keeper.get_chunk_info(*itor);

			types::coords_t<> coords = { __xaxis, __yaxis, __zaxis };
			if (maths::is_inside(coords, chunk_info.coords,
			this-> _chunk_keeper.get_chunk_xlen(),
			this-> _chunk_keeper.get_chunk_ylen(),
			this-> _chunk_keeper.get_chunk_zlen())) return *itor;
		}
	}

	boost::int8_t de_init() {
		this-> _chunk_keeper.de_init();
	}

	private:
	std::set<uint_t *> chunk_index;
	chunk_keeper _chunk_keeper;
};
}
}


# endif /*__chunk__manager__hpp*/
