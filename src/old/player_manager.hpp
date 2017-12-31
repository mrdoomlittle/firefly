# ifndef __player__manager__hpp
# define __player__manager__hpp
# include "types/id_t.hpp"
# include "types/err_t.h"
# include "types/player_t.hpp"
# include <cstdio>
# include <set>
# include "system/errno.h"
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
namespace mdl {
namespace firefly {
class player_manager {
	public:
	types::err_t add_player(types::__id_t& __player_id);
	types::err_t rm_player(types::__id_t __player_id);

	bool null_player_id(types::__id_t __player_id) {
		return __player_id >= this-> player_indx.size()? true : false;
	}

	uint_t player_count() {
		return this-> player_c;
	}

	private:
	uint_t player_c = 0;
	std::set<types::__id_t> unused_ids;
	ublas::vector<types::player_t> player_indx;
};
}
}


# endif /*__player__manager__hpp*/
