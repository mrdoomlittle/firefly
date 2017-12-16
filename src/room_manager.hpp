# ifndef __ffly__room__manager__hpp
# define __ffly__room__manager__hpp
# include "types/err_t.h"
# include "system/vec.h"
# include "system/arr.h"
# include "room.hpp"
# include "types/no_t.h"
# define MAX_ROOMS 20
namespace mdl {
namespace firefly {
class room_manager {
	public:
	room_manager() : spare_rooms(VEC_AUTO_RESIZE){}
	types::err_t creat_room(types::no_t*& __no);
	types::err_t del_room(types::no_t *__no);
	private:
	system::vec<types::no_t*> spare_rooms;
	system::arr<room*, MAX_ROOMS> rooms;
	uint_t no_rooms = 0;
};
}
}
# endif /*__ffly__room__manager__hpp*/
