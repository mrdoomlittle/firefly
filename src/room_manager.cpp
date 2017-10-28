# include "room_manager.hpp"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "data/mem_cpy.h"
# include "system/io.h"
mdl::firefly::types::err_t mdl::firefly::room_manager::creat_room(types::no_t*& __no, types::bool_t __owr) {
	printf("%u - %u\n", this->no_spare, this->no_rooms);
	if (this->spare_rooms != nullptr) {
		__no = *(this->spare_rooms+(this->no_spare-1));
		if (!(this->no_spare-1)) {
			memory::mem_free(this->spare_rooms);
			this->spare_rooms = nullptr;
			this->no_spare = 0;
		} else {
			if ((this->spare_rooms = (types::no_t**)memory::mem_realloc(this->spare_rooms, (--this->no_spare)*sizeof(types::no_t*))) == nullptr) {
				// err
			}
		}
		return FFLY_SUCCESS;
	}

	if (!this->rooms) {
		if ((this->rooms = (room_manager::room*)memory::mem_alloc(sizeof(room_manager::room))) == nullptr) {
			//err
		}
		this->no_rooms++;
	} else {
		if ((this->rooms = (room_manager::room*)memory::mem_realloc(this->rooms, (++no_rooms)*sizeof(sizeof(room_manager::room)))) == nullptr) {
			// err
		}
	}

	room_manager::room *room = this->rooms+(this->no_rooms-1);
	room->no = (types::no_t*)memory::mem_alloc(sizeof(types::no_t));
	__no = room->no;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::room_manager::del_room(types::no_t *__no, types::bool_t __hard) {
	if (!__hard) {
		if (!this->spare_rooms) {
			if ((this->spare_rooms = (types::no_t**)memory::mem_alloc(sizeof(types::no_t*))) == nullptr) {
				// err
			}
			this->no_spare++;
		} else
			this->spare_rooms = (types::no_t**)memory::mem_realloc(this->spare_rooms, (++this->no_spare)*sizeof(types::no_t*));
		*(this->spare_rooms+(this->no_spare-1)) = __no;
	} else {
		if (!(this->no_rooms-1)) {
			memory::mem_free(this->rooms);
			this->rooms = nullptr;
			this->no_rooms = 0;
		} else {
			if (*__no == this->no_rooms-1) {
				data::mem_cpy(this->rooms+*__no, this->rooms+(this->no_rooms-1), sizeof(room_manager::room));
				*(this->rooms+*__no)->no = *__no;
			}

			this->rooms = (room_manager::room*)realloc(this->rooms, (--this->no_rooms)*sizeof(room_manager::room));
		}
		memory::mem_free(__no);
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::room_manager::manage() {

}
