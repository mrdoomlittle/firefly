# ifndef __ffly__room__manager__hpp
# define __ffly__room__manager__hpp
# include <mdlint.h>
# include "types/err_t.h"
# include "types/no_t.h"
# include "types/pixelmap_t.h"
# include "types/bool_t.h"
namespace mdl {
namespace firefly {
class room_manager {
	struct room {
		types::no_t *no;
		mdl_uint_t xa_len, ya_len;
		types::pixelmap_t pixelbuff;
	};

	public:
	types::err_t creat_room(types::no_t*& __no, types::bool_t __owr);
	types::err_t del_room(types::no_t *__no, types::bool_t __hard);
	types::err_t manage();

	private:
	types::no_t *cur_room = nullptr;
	room *rooms = nullptr;
	types::no_t **spare_rooms = nullptr;
	mdl_uint_t no_rooms = 0, no_spare = 0;
};
}
}

# endif /*__ffly__room__manager__hpp*/
