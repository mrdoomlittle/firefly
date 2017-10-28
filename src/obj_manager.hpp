# ifndef __ffly__obj__manager__hpp
# define __ffly__obj__manager__hpp
# include "types/err_t.h"
# include "types/pixelmap_t.h"
# include "types/id_t.h"
# include "system/vec.h"
# include "types/obj_t.h"
namespace mdl {
namespace firefly {
class obj_manager {
	public:
	obj_manager() : objs(VEC_AUTO_RESIZE) {}
	types::err_t draw_objs(types::pixelmap_t __pixelbuff);
	types::err_t creat_obj(types::obj_t*& __obj, types::id_t& __id);
	types::err_t add_obj(types::obj_t *__obj, types::id_t __id);

	private:
	system::vec<obj_t*> objs;
};
}
}


# endif /*__ffly__obj__manager__hpp*/
