# include "room_manager.hpp"
boost::int8_t mdl::firefly::room_manager::add_room(uint_t*& __room_id, bool __overwrite) {
	if (__room_id != nullptr && !__overwrite) return FFLY_NOP;

	if (this-> room_count == 0) {
		if ((this-> _room_info = (room_info_t *)memory::mem_alloc(sizeof(room_info_t))) == NULL) {
			fprintf(stderr, "room_manager: failed to alloc memory for 'room_info', errno: %d\n", errno);
			return FFLY_FAILURE;
		}

		if ((this-> _room_data = (room_data_t *)memory::mem_alloc(sizeof(room_data_t))) == NULL) {
			fprintf(stderr, "room_manager: failed to alloc memory for 'room_data', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
			room_info_t *room_info = NULL;
			if ((room_info = (room_info_t *)memory::mem_realloc(this-> _room_info, (this-> room_count + 1) * sizeof(room_info_t))) == NULL) {
				fprintf(stderr, "room_manager: failed to realloc memory for 'room_info', errno: %d\n", errno);
				return FFLY_FAILURE;
			} else
				this-> _room_info = room_info;

			room_data_t *room_data = NULL;
			if ((room_data = (room_data_t *)memory::mem_realloc(this-> _room_data, (this-> room_count + 1) * sizeof(room_data_t))) == NULL) {
				fprintf(stderr, "room_manager: failed to realloc memory for 'room_info', errno: %d\n", errno);
				room_info = NULL;
				if ((room_info = (room_info_t *)memory::mem_realloc(this-> _room_info, (this-> room_count - 1) * sizeof(room_info_t))) == NULL)
					return FFLY_FAILURE;
				else
					this-> _room_info = room_info;
			} else
				this-> _room_data = room_data;
	}

	room_info_t *room_info = nullptr;
	room_data_t *room_data = nullptr;

	if (!this-> unused_ids.empty()) {
		std::set<uint_t *>::iterator itor = this-> unused_ids.end();
		--itor;

		__room_id = *itor;
		this-> unused_ids.erase(itor);
	} else {
		this-> _room_info[this-> room_count].id = __room_id = (uint_t *)memory::mem_alloc(sizeof(uint_t));
		if (__room_id == NULL) {
			fprintf(stderr, "room_manager: failed to alloc memory for 'room_id', errno: %d\n", errno);
			goto mem_clean;
		} else
			*__room_id = this-> room_count;
	}

	room_info = &this-> _room_info[*__room_id];
	room_data = &this-> _room_data[*__room_id];

	gui::btn_manager *btn_manager;
	if ((btn_manager = (gui::btn_manager *)memory::mem_alloc(sizeof(gui::btn_manager))) == NULL) {
		fprintf(stderr, "room_manager: failed to alloc memory for 'btn_manager', errno: %d\n", errno);
		goto mem_clean;
	} else {
		uint_t pb_xaxis_len = 0, pb_yaxis_len = 0;
		if (this-> use_glob_pb_size) {
			pb_xaxis_len = this-> pb_xaxis_len;
			pb_yaxis_len = this-> pb_yaxis_len;
		}

		room_data-> btn_manager = new (btn_manager) gui::btn_manager(nullptr, &this-> wd_coords, &this-> mouse_coords, pb_xaxis_len, pb_yaxis_len);
		if (this-> window != nullptr) {
			room_data-> btn_manager-> mouse_pressed = &this-> window-> button_press();
			room_data-> btn_manager-> mouse_btn_id = &this-> window-> button_code();
 		}
	}

	this-> room_count ++;

	return FFLY_SUCCESS;
	mem_clean:

	if (__room_id != NULL)
		memory::mem_free(__room_id);

	if (this-> room_count == 0) {
		memory::mem_free(this-> _room_info);
		memory::mem_free(this-> _room_data);
	} else {
		this-> _room_info = (room_info_t *)memory::mem_realloc(this-> _room_info, (this-> room_count - 1) * sizeof(room_info_t));
		this-> _room_data = (room_data_t *)memory::mem_realloc(this-> _room_data, (this-> room_count - 1) * sizeof(room_data_t));
	}

	return FFLY_FAILURE;
}

boost::int8_t mdl::firefly::room_manager::create_btn(uint_t *__room_id, uint_t& __btn_id, types::pixmap_t __pixmap, types::coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len) {
//	room_info_t& room_info = this-> _room_info[*__room_id];
	room_data_t& room_data = this-> _room_data[*__room_id];

	__btn_id = room_data.btn_manager-> create_btn(__pixmap, __coords, __xaxis_len, __yaxis_len);


	return FFLY_SUCCESS;
}

// move to 'manage()'
boost::int8_t mdl::firefly::room_manager::draw_room(uint_t *__room_id) {
	uint_t *room_id = __room_id == nullptr? this-> curr_room_id : __room_id;
	if (room_id == nullptr) return FFLY_FAILURE;

	room_info_t& room_info = this-> _room_info[*room_id];
	room_data_t& room_data = this-> _room_data[*room_id];

	types::pixmap_t pixbuff = room_data.pixbuff == nullptr? this-> pixbuff : room_data.pixbuff;
	if (pixbuff == nullptr) return FFLY_NOP;

	if (room_data.btn_manager != nullptr) {
		if (room_data.btn_manager-> manage(pixbuff) == FFLY_FAILURE) {
			fprintf(stderr, "room_manager: failed to call 'btn_manager::manage'\n");
			return FFLY_FAILURE;
		}
	}

	return FFLY_SUCCESS;
}

boost::int8_t mdl::firefly::room_manager::rm_room(uint_t *__room_id, bool __hard) {
	return FFLY_NOP;
}

boost::int8_t mdl::firefly::room_manager::manage(uint_t *__room_id) {
	if (!this-> room_change.empty()) {
		printf("room_manager: changing room to id with %d\n", *this-> room_change.front());
		this-> curr_room_id = this->room_change.front();
		this->room_change.pop();
	}

	uint_t *room_id = __room_id == nullptr? this-> curr_room_id : __room_id;
	if (room_id == nullptr) return FFLY_FAILURE;

	this-> wd_coords = this-> window-> get_wd_coords();
	this-> mouse_coords = this-> window-> get_mouse_coords().wd;

	// this will be removed a later version
	if (this-> draw_room() == FFLY_FAILURE) {
		fprintf(stderr, "room_manager: failed to draw.\n");
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}

boost::int8_t mdl::firefly::room_manager::de_init() {
	printf("room_manager: going to free all memory for rooms\n");
	for (uint_t _room_id = 0; _room_id != this-> room_count; _room_id ++) {
		uint_t *room_id = (this-> _room_info + _room_id)-> id;
		if (!this-> id_inuse(room_id)) continue;

		room_info_t& room_info = this-> _room_info[*room_id];
		room_data_t& room_data = this-> _room_data[*room_id];

		room_data.btn_manager-> de_init();
		memory::mem_free(room_data.btn_manager);

		memory::mem_free(room_info.id);
	}

	return FFLY_SUCCESS;
}
