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
namespace mdl {
namespace firefly {
class obj_manager;
class gravy_manager {
	public:
	boost::int8_t init(uint_t __obj_c);
	boost::int8_t manage(obj_manager *__obj_mana_ptr, uint_t __obj_id, uint_t __gravity_speed, bool __gravity_enabled);

	private:
	uint_t obj_c = 0;
	system::stop_watch obj_timer[BLOCK_SIZE];
} ;

}
}

# endif /*__gravy__manager__hpp*/
