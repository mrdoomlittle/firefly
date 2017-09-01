# ifndef __gravy__manager__hpp
# define __gravy__manager__hpp
# include "ffly_config.hpp"
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
# include "system/stop_watch.hpp"
# include "types/obj_info_t.hpp"
# include "types/pixmap_t.h"
# include "system/errno.h"
# include "obj_manager.hpp"
# include <utility>
# include <set>
# include "maths/cal_dist.hpp"
# include "maths/squar_rt.hpp"
# include "flip_dir.hpp"
# include "ffly_config.hpp"
# include "types/err_t.h"
namespace mdl {
namespace firefly {
class obj_manager;
class gravy_manager {
	public:
	types::err_t init(uint_t __obj_c);
	types::err_t manage(obj_manager *__obj_manager, uint_t __obj_id, uint_t __gravy_speed, bool __gravy_enabled);
	types::err_t de_init();

	void add_to_pool(uint_t __obj_id, uint_t __other_id) {
		printf("added obj with id: %d to a obj pool.\n", __other_id);
		this-> gravy_pool[__obj_id].resize(this-> gravy_pool[__obj_id].size() + 1);
		this-> gravy_pool[__obj_id][this-> gravy_pool[__obj_id].size() - 1].obj_id = __other_id;
	}

	typedef struct {
		bool inited = false;
		uint_t obj_id;
		system::stop_watch timer;
	} pool_ele_t;

	private:
	uint_t obj_c = 0;
	ublas::vector<pool_ele_t> gravy_pool[BLOCK_SIZE];
	system::stop_watch obj_timer[BLOCK_SIZE];
} ;

}
}

# endif /*__gravy__manager__hpp*/
