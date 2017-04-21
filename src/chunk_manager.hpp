# ifndef __chunk__manager__hpp
# define __chunk__manager__hpp
# include "chunk_keeper.hpp"
# include <eint_t.hpp>
# include <set>
# include "types/id_t.hpp"
# include "types/err_t.h"
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

	types::err_t add_cnk(types::id_t& __cnk_id, uint_t __xaxis, uint_t __yaxis, uint_t __zaxis);
	types::err_t rm_cnk(uint_t __xaxis, uint_t __yaxis, uint_t __zaxis);

	types::id_t find_cnk_id(uint_t __xaxis, uint_t __yaxis, uint_t __zaxis) {
		for (std::set<uint_t *>::iterator itor = this-> cnk_indx.begin(); itor != this-> cnk_indx.end(); ++itor) {
			types::chunk_info_t cnk_info = this-> cnk_info(*itor);
			if (cnk_info.coords.xaxis == __xaxis && cnk_info.coords.yaxis == __yaxis && cnk_info.coords.zaxis == __zaxis) {
				return *itor;
			}
		}
	}

	types::id_t coords_to_id(uint_t __xaxis, uint_t __yaxis, uint_t __zaxis) {
		for (std::set<uint_t *>::iterator itor = this-> cnk_indx.begin(); itor != this-> cnk_indx.end(); ++itor) {
			types::chunk_info_t cnk_info = this-> cnk_info(*itor);

			types::coords_t<> coords = { __xaxis, __yaxis, __zaxis };
			if (maths::is_inside(coords, cnk_info.coords,
			this-> _chunk_keeper.get_cnk_xlen(),
			this-> _chunk_keeper.get_cnk_ylen(),
			this-> _chunk_keeper.get_cnk_zlen())) return *itor;
		}
	}

	types::err_t de_init() {this-> _chunk_keeper.de_init();}
	__inline__ types::chunk_info_t& cnk_info(types::id_t __cnk_id) {
		return this-> _chunk_keeper.get_cnk_info(__cnk_id);
	}
	//__inline__ types::chunk_info_t get_cnk_info(types::id_t __cnk_id) {}

	__inline__ types::chunk_data_t& cnk_data(types::id_t __cnk_id) {
		return this-> _chunk_keeper.get_cnk_data(__cnk_id);
	}
	//__inline__ types::chunk_data_t get_cnk_data(types::id_t __cnk_id) {}

	__inline__ uint_t get_cnk_xlen() { return this-> _chunk_keeper.get_cnk_xlen(); }
	__inline__ uint_t get_cnk_ylen() { return this-> _chunk_keeper.get_cnk_ylen(); }
	__inline__ uint_t get_cnk_zlen() { return this-> _chunk_keeper.get_cnk_zlen(); }
	private:
	std::set<types::id_t> cnk_indx;
	chunk_keeper _chunk_keeper;
};
}
}


# endif /*__chunk__manager__hpp*/
