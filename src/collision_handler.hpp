# ifndef __collision__handler__hpp
# define __collision__handler__hpp
# include "ffly_config.hpp"
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
# include <set>
# include "types/coords_t.hpp"
namespace mdl {
namespace firefly {
namespace physics {
class collision_handler {
	void handler();

	void begin_in_thread();

	typedef struct {
		uint_t obj_id;
		types::coords_t<>*coords;
	} collis_pool_t;

	private:
	std::set<uint_t> collision_pool[BLOCK_SIZE];
};

}
}
}

# endif /*__collision__handler__hpp*/
