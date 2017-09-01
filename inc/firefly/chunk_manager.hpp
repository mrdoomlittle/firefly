# ifndef __chunk__manager__hpp
# define __chunk__manager__hpp
# include <eint_t.h>
# include <set>
# include "types/id_t.h"
# include "types/err_t.h"
# include "types/coords_t.h"
# include "chunk_info_t.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "maths/is_inside.hpp"
# include "chunk_keeper.h"
# include "system/set.hpp"
namespace mdl {
namespace firefly {
class chunk_manager {
	public:
	types::err_t init(uint_t __xa_len, uint_t __ya_len, uint_t __za_len);
	types::err_t de_init();

	chunk_manager(uint_t __xa_len, uint_t __ya_len, uint_t __za_len) : ck(__xa_len, __ya_len, __za_len) {}

	types::err_t add_cnk(types::id_t& __cnk_id, uint_t __xa, uint_t __ya, uint_t __za);
	types::err_t rm_cnk(uint_t __xa, uint_t __ya, uint_t __za);

	types::id_t find_cnk_id(uint_t __xa, uint_t __ya, uint_t __za) {
		for (system::set<uint_t*>::iterator itr = this-> cnk_indx.begin(); itr != this-> cnk_indx.end(); itr++) {
			chunk_info cnk_info = this->cnk_info(*itr);
			if (cnk_info.coords.xa == __xa && cnk_info.coords.ya == __ya && cnk_info.coords.za == __za) {
				return *itr;
			}
		}
	}

	types::id_t coords_to_id(uint_t __xa, uint_t __ya, uint_t __za) {
		for (system::set<uint_t*>::iterator itr = this-> cnk_indx.begin(); itr != this-> cnk_indx.end(); itr++) {
			chunk_info cnk_info = this-> cnk_info(*itr);

			types::coords_t<> coords = {__xa, __ya, __za};
			if (maths::is_inside(coords, cnk_info.coords, this->ck.get_xa_len(), this->ck.get_ya_len(), this->ck.get_za_len())) return *itr;
		}
	}

	chunk_info __inline__& cnk_info(types::id_t __id) {return this->ck.get_info(__id);}
	chunk_data __inline__& cnk_data(types::id_t __id) {return this->ck.get_data(__id);}

	uint_t __inline__ get_cnk_xlen() {return this->ck.get_xa_len();}
	uint_t __inline__ get_cnk_ylen() {return this->ck.get_ya_len();}
	uint_t __inline__ get_cnk_zlen() {return this->ck.get_za_len();}
	private:
	system::set<types::id_t> cnk_indx;
	chunk_keeper ck;
};
}
}


# endif /*__chunk__manager__hpp*/
