# include "room_manager.hpp"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include <new>
# include "system/io.h"
mdl::firefly::types::err_t mdl::firefly::room_manager::creat_room(types::no_t*& __no) {
	if (this->spare_rooms.size() > 0) {
		types::err_t err;
		__no = this->spare_rooms.pop_back(err);
	} else {
		__no = static_cast<types::no_t*>(memory::mem_alloc(sizeof(types::no_t)));
		*__no = no_rooms++;
	}
	room *r = static_cast<room*>(memory::mem_alloc(sizeof(room)));
	new (r) room();
	system::io::fprintf(ffly_log, "room-manager: adding room with assigned no %u\n", *__no);
	this->rooms.put(r, *__no);
	return FFLY_SUCCESS;
}

mdl::firefly::room* mdl::firefly::room_manager::get_room(types::no_t *__no) {
	return this->rooms.at(*__no);
}

mdl::firefly::types::err_t mdl::firefly::room_manager::del_room(types::no_t *__no) {
	system::io::fprintf(ffly_log, "room-manager: deleting room %u\n", *__no);
	room *r = this->rooms.get(*__no);
	r->~room();

	this->spare_rooms.push_back(__no);

	memory::mem_free(r);
	no_rooms--;
}
