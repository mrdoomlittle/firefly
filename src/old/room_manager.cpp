# include "room_manager.hpp"
mdl::firefly::types::id_t *mdl::firefly::room_manager::_room_id = nullptr;

void (* mdl::firefly::room_manager::_btn_press)(uint_t, int, void *, types::id_t) = nullptr;
void (* mdl::firefly::room_manager::_btn_hover)(uint_t, void *, types::id_t) = nullptr;

mdl::firefly::room_manager::room_manager() {room_manager::_room_id = &this-> curr_room_id;}
void mdl::firefly::room_manager::btn_pressf(uint_t __btn_id, int __mbtn_id, void *__voidptr) {
	room_manager::_btn_press(__btn_id, __mbtn_id, __voidptr, *room_manager::_room_id);}

void mdl::firefly::room_manager::btn_hoverf(uint_t __btn_id, void *__voidptr) {
	room_manager::_btn_hover(__btn_id, __voidptr, *room_manager::_room_id);}

void (* mdl::firefly::room_manager::btn_press(void (* __btn_press)(uint_t, int, void *, types::id_t)))(uint_t, int, void *) {
	room_manager::_btn_press = __btn_press;
	return &room_manager::btn_pressf;
}

void (* mdl::firefly::room_manager::btn_hover(void (* __btn_hover)(uint_t, void *, types::id_t)))(uint_t, void *) {
	room_manager::_btn_hover = __btn_hover;
	return &room_manager::btn_hoverf;
}

bool mdl::firefly::room_manager::btn_event_poll(types::btn_event_t& __btn_event, uint_t& __event_type) {
	if (this-> btn_event_pool.empty()) return false;
	__btn_event = this-> btn_event_pool.front().first;
	__event_type = this-> btn_event_pool.front().second;
	this-> btn_event_pool.pop();
	return true;
}

mdl::firefly::types::err_t mdl::firefly::room_manager::add_room(types::id_t& __room_id, bool __overwrite) {
	if (__room_id != nullptr && !__overwrite) return FFLY_NOP;

	if (this-> room_count == 0) {
		if ((this-> _room_info = (room_info_t *)memory::mem_alloc(sizeof(room_info_t))) == NULL) {
			system::io::printf(stderr, "room_manager: failed to alloc memory for 'room_info', errno: %d\n", errno);
			return FFLY_FAILURE;
		}

		if ((this-> _room_data = (room_data_t *)memory::mem_alloc(sizeof(room_data_t))) == NULL) {
			fprintf(stderr, "room_manager: failed to alloc memory for 'room_data', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
			room_info_t *room_info = NULL;
			if ((room_info = (room_info_t *)memory::mem_realloc(this-> _room_info, (this-> room_count + 1) * sizeof(room_info_t))) == NULL) {
				system::io::printf(stderr, "room_manager: failed to realloc memory for 'room_info', errno: %d\n", errno);
				return FFLY_FAILURE;
			} else
				this-> _room_info = room_info;

			room_data_t *room_data = NULL;
			if ((room_data = (room_data_t *)memory::mem_realloc(this-> _room_data, (this-> room_count + 1) * sizeof(room_data_t))) == NULL) {
				system::io::printf(stderr, "room_manager: failed to realloc memory for 'room_info', errno: %d\n", errno);
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
		std::set<types::id_t>::iterator itor = this-> unused_ids.end();
		--itor;

		__room_id = *itor;
		this-> unused_ids.erase(itor);
	} else {
		this-> _room_info[this-> room_count].id = __room_id = (types::id_t)memory::mem_alloc(sizeof(types::__id_t));
		if (__room_id == NULL) {
			system::io::printf(stderr, "room_manager: failed to alloc memory for 'room_id', errno: %d\n", errno);
			goto mem_clean;
		} else
			*__room_id = this-> room_count;
	}

	room_info = &this-> _room_info[*__room_id];
	room_data = &this-> _room_data[*__room_id];

	gui::btn_manager *btn_manager;
	if ((btn_manager = (gui::btn_manager *)memory::mem_alloc(sizeof(gui::btn_manager))) == NULL) {
		system::io::printf(stderr, "room_manager: failed to alloc memory for 'btn_manager', errno: %d\n", errno);
		btn_manager = nullptr;
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

# ifdef __RM_LAYERING
	layer_manager *_layer_manager;
	if ((_layer_manager = (layer_manager *)memory::mem_alloc(sizeof(layer_manager))) == NULL) {
		system::io::printf(stderr, "room_manager: failed to alloc memory for 'layer_manager', errno: %d\n", errno);
		btn_manager = nullptr;
		goto mem_clean;
	} else
		room_data-> _layer_manager = new (_layer_manager) layer_manager();
# endif

	room_data-> pixbuff = nullptr;

	this-> room_count ++;

	return FFLY_SUCCESS;
	mem_clean:

	if (__room_id != NULL) memory::mem_free(__room_id);
	if (btn_manager != nullptr) memory::mem_free(btn_manager);

	if (this-> room_count == 0) {
		memory::mem_free(this-> _room_info);
		memory::mem_free(this-> _room_data);
	} else {
		this-> _room_info = (room_info_t *)memory::mem_realloc(this-> _room_info, (this-> room_count - 1) * sizeof(room_info_t));
		this-> _room_data = (room_data_t *)memory::mem_realloc(this-> _room_data, (this-> room_count - 1) * sizeof(room_data_t));
	}

	return FFLY_FAILURE;
}

mdl::firefly::types::err_t mdl::firefly::room_manager::create_btn(types::id_t __room_id, uint_t& __btn_id, types::pixmap_t __pixmap, types::_2d_coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len) {
//	room_info_t& room_info = this-> _room_info[*__room_id];
	room_data_t& room_data = this-> _room_data[*__room_id];
	types::err_t any_err;

	if ((any_err = __btn_id = room_data.btn_manager-> create_btn(__pixmap, __coords, __xaxis_len, __yaxis_len)) != FFLY_SUCCESS) return any_err;
	return FFLY_SUCCESS;
}

// move to 'manage()'
mdl::firefly::types::err_t mdl::firefly::room_manager::draw_room(types::id_t __room_id) {
	types::id_t room_id = __room_id == nullptr? this-> curr_room_id : __room_id;
	if (!room_id) return FFLY_NOP;

	room_info_t& room_info = this-> _room_info[*room_id];
	room_data_t& room_data = this-> _room_data[*room_id];

	types::pixmap_t pixbuff = room_data.pixbuff == nullptr? this-> pixbuff : room_data.pixbuff;
	if (!pixbuff) return FFLY_NOP;

	/*
		if __room_id has been set this will cause issues with the static btn_press, and btn_hover
		as they depend on curr_room_id to be set.
	*/

	types::id_t tmp_room_id;
	if (room_id == __room_id) {
		tmp_room_id = this-> curr_room_id;
		this-> curr_room_id = room_id;
	}

	mdl::firefly::types::err_t any_err;
	if (room_data.btn_manager != nullptr) {
		if ((any_err = room_data.btn_manager-> manage(pixbuff)) != FFLY_SUCCESS) {
			system::io::printf(stderr, "room_manager: failed to call 'btn_manager::manage'\n");
			return any_err;
		}
	}

# ifdef __RM_LAYERING
	if (room_data._layer_manager) {
		if ((any_err = room_data._layer_manager-> draw_layers(pixbuff, pb_xaxis_len, pb_yaxis_len)) != FFLY_SUCCESS) {
			system::io::printf(stderr, "room_manager: failed to draw layers.\n");
			return any_err;
		}
	}
# endif

	if (room_id == __room_id)
		this-> curr_room_id = tmp_room_id;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::room_manager::rm_room(types::id_t __room_id, bool __hard) {
	return FFLY_NOP;
}

mdl::firefly::types::err_t mdl::firefly::room_manager::manage(types::id_t __room_id) {
	if (!this-> room_change.empty() && !this-> window-> is_button_press()) {
		system::io::printf(stdout, "room_manager: changing room to id with %d\n", *this-> room_change.front());
		this-> curr_room_id = this->room_change.front();
		this-> room_change.pop();
	}

	types::id_t room_id = __room_id == nullptr? this-> curr_room_id : __room_id;
	if (!room_id) return FFLY_NOP;

	this-> wd_coords = this-> window-> get_wd_coords();
	this-> mouse_coords = this-> window-> get_mouse_coords().wd;

	gui::btn_manager *btn_manager = this-> _room_data[*room_id].btn_manager;
	if (btn_manager != nullptr) {
		if (!btn_manager-> event_queue.empty()) {
			this-> btn_event_pool.push(btn_manager-> event_queue.front());
			btn_manager-> event_queue.pop();
		}
	}

	types::err_t any_err;
	if ((any_err = this-> draw_room()) != FFLY_SUCCESS) {
		system::io::printf(stderr, "room_manager: failed to draw room.\n");
		return any_err;
	}

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::room_manager::de_init() {
	system::io::printf(stdout, "room_manager: going to free all memory for rooms\n");
	for (uint_t _room_id = 0; _room_id != this-> room_count; _room_id ++) {
		uint_t *room_id = (this-> _room_info + _room_id)-> id;
		if (!this-> id_inuse(room_id)) continue;

		room_info_t& room_info = this-> _room_info[*room_id];
		room_data_t& room_data = this-> _room_data[*room_id];

		room_data.btn_manager-> de_init();
# ifdef __RM_LAYERING
		room_data._layer_manager-> de_init();
# endif
		memory::mem_free(room_data.btn_manager);

		memory::mem_free(room_info.id);
	}

	return FFLY_SUCCESS;
}