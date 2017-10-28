# include "obj_manager.hpp"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/thread.h"
extern obj_handle();

mdl::firefly::types::err_t mdl::firefly::obj_manager::draw_objs(types::pixelmap_t __pixelbuff) {

}

mdl::firefly::types::err_t mdl::firefly::obj_manager::creat_obj(types::obj_t*& __obj, types::id_t& __id) {
	__obj = (types::obj_t*)memory::mem_alloc(sizeof(types::obj_t));
	__id = (types::id_t)memory::mem_alloc(sizeof(types::__id_t));
	// prep for adding
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::obj_manager::add_obj(types::obj_t *__obj, types::id_t __id) {
	*__id = this->obj.size();
	this->objs.push_back(__obj);

	system::thread t(obj_handle);
}
