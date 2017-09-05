# ifndef __chunk__keeper__hpp
# define __chunk__keeper__hpp
# include "ffly_config.hpp"
# include <mdlint.h>
# include <boost/cstdint.hpp>
# include <cstdio>
# include <set>
# include <errno.h>
# include <string.h>
# include "types/err_t.h"
# include "types/id_t.hpp"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/alloc_pixmap.hpp"
# include "memory/mem_realloc.h"
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

	types::err_t create_cnk(types::id_t& __cnk_id, bool __overwrite = false);
	types::err_t del_cnk(types::id_t __cnk_id, bool __hard = false);

	types::err_t de_init() {
		for (uint_t cnk_id{}; cnk_id != this->cnk_c; cnk_id++) {
			if (this->unused_ids.find(this->cnk_info[cnk_id].id) != this->unused_ids.end()) {
				memory::mem_free(this->cnk_info[cnk_id].id);
				continue;
			}

			types::chunk_info_t& cnk_info = this->cnk_info[cnk_id];
			types::chunk_data_t& cnk_data = this->cnk_data[cnk_id];

			memory::mem_free(cnk_info.id);
			memory::mem_free(cnk_data.particles);
			memory::mem_free(cnk_data._1d_pm);
		}
		return FFLY_SUCCESS;
	}

	types::chunk_info_t __inline__& get_cnk_info(types::id_t __cnk_id) {
		return this->cnk_info[*__cnk_id];
	}

	types::chunk_data_t __inline__& get_cnk_data(types::id_t __cnk_id) {
		return this->cnk_data[*__cnk_id];
	}

	uint_t __inline__ get_cnk_xlen() {return this->xaxis_len*UNI_PAR_XLEN;}
	uint_t __inline__ get_cnk_ylen() {return this->yaxis_len*UNI_PAR_YLEN;}
	uint_t __inline__ get_cnk_zlen() {return this->zaxis_len*UNI_PAR_ZLEN;}
	private:
	std::set<types::id_t> unused_ids;
	uint_t cnk_c = 0;
	types::chunk_info_t *cnk_info = nullptr;
	types::chunk_data_t *cnk_data = nullptr;
	uint_t const xaxis_len, yaxis_len, zaxis_len;
};
}
}

# endif /*__chunk__keeper__hpp*/
