# include "ffly_client.hpp"
bool mdl::ffly_client::to_shutdown = false;
mdl::firefly::types::err_t mdl::ffly_client::connect_to_server(int& __sockd) {
	for (; this->connect_trys != FFLY_MAX_CONN_TRYS; this->connect_trys++) {
		if (this->connect_trys != 0) {
			firefly::system::io::printf(stderr, "failed to connected to server, attempt: %d\n", this->connect_trys);
			usleep(FFLY_DELAY_BT_TRYS);
		}

		this->server_connected = true;

		if (this->tcp_stream.init(this->server_ipaddr, 21299, __sockd) == FFLY_FAILURE) {
			this->server_connected = false;
			firefly::system::io::printf(stderr, "failed to init tcp stream, errno: %d\n", errno);
			continue;
		}

		if (this->tcp_stream.connect() == FFLY_FAILURE) {
			this->server_connected = false;
			firefly::system::io::printf(stderr, "failed to connect to server thru tcp stream, errno: %d\n", errno);
			continue;
		}

		if (this->udp_stream.init(this->server_ipaddr, 10198) == FFLY_FAILURE) {
			this->server_connected = false;
			firefly::system::io::printf(stderr, "failed to init udp stream, errno: %d\n",  errno);
			continue;
		}

		if (server_connected) {
			this->connect_trys = 0;
			this->server_connected = true;
			return FFLY_SUCCESS;
		}
	}

	return FFLY_FAILURE;
}

mdl::firefly::types::err_t mdl::ffly_client::send_client_info() {
	static serializer serialize('\0');
	static std::size_t size = serialize.get_size(&this->client_info);
	static bool inited = false;

	if (!inited) {
		serialize.init(size);
		serialize | 'r';
	}

	this->client_info.achieve(serialize);
	serialize.reset();

	if (this->tcp_stream.send(serialize.get_serial(), size) == FFLY_FAILURE) {
		firefly::system::io::printf(stderr, "failed to send client info to server, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::ffly_client::recv_cam_frame() {
	if (this->udp_stream.recv(this->layer.get_layer_pixmap(this->cam_layer_id), this->cam_pm_size) == FFLY_FAILURE) {
		firefly::system::io::printf(stderr, "failed to recv camera feed from server, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}

void static ctrl_c(int __sig) {
	mdl::firefly::system::io::printf(stdout, "looks like ctrl_c was called bye bye...\n");
	mdl::ffly_client::to_shutdown = true;
}

mdl::firefly::types::err_t mdl::ffly_client::init(firefly::types::init_opt_t __init_options) {
	firefly::types::err_t any_err;
	if ((any_err = ffly_system::init()) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_client: failed to init ffly system, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}

	if ((any_err = ffly_graphics::init()) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_client: failed to init ffly graphics, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}

	struct sigaction sig_handler;

	sig_handler.sa_handler = ctrl_c;

	sigemptyset(&sig_handler.sa_mask);

	sig_handler.sa_flags = 0;
	sigaction(SIGINT, &sig_handler, NULL);

	if (!__init_options.cam_xlen % 2 || !__init_options.cam_ylen % 2) {
		firefly::system::io::printf(stderr, "failed to set the camera xa and ya lengths, one of them is not divisible by 2.\n");
		return FFLY_FAILURE;
	}

	this->init_options = __init_options;

	// camera
	this->cam_xlen = __init_options.cam_xlen;
	this->cam_ylen = __init_options.cam_ylen;
	this->cam_pm_size = this->cam_xlen*this->cam_ylen*4;

	if (__init_options.add_bse_layer) {
		this->bse_layer_id = this->layer.add_layer(this->wd_xa_len, this->wd_ya_len, 0, 0);
		this->layer.lock_layer(this->bse_layer_id);
	}

# ifdef __WITH_UNI_MANAGER
    if (__init_options.uni_init) {
		if ((any_err = this->uni_manager.init(4, 4, 2)) != FFLY_SUCCESS) {
			firefly::system::io::printf(stderr, "ffly_client: failed to init uni manager, ffly_errno: %d\n");
			return any_err;
		}
	}
# endif

# if defined(__WITH_OBJ_MANAGER) && defined(__WITH_UNI_MANAGER)
	if (!__init_options.obj_manger_ptr) {
		//uint_t layer_id = this->layer.add_layer(this->wd_xa_len, this->wd_ya_len, 0, 0);
		static firefly::obj_manager _obj_manager(&this->uni_manager);//(this->layer.get_layer_pixmap(layer_id), this->wd_xa_len, this->wd_ya_len, 1);
		this->obj_manager = &_obj_manager;
		if ((any_err = this->entity_manager.init(&_obj_manager, &this->uni_manager)) != FFLY_SUCCESS) {
			firefly::system::io::printf(stderr, "ffly_client: failed to init entity manager, ffly_errno: %d\n", ffly_errno);
			return any_err;
		}
	}
# endif

# ifdef ROOM_MANAGER
	if ((any_err = this->room_manager.init(&window)) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_client: failed to init room manager, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}

	// room manager
	this->room_manager.use_glob_pb_size = true;
	this->room_manager.set_glob_pb_xlen(this->wd_xa_len);
	this->room_manager.set_glob_pb_ylen(this->wd_ya_len);
	if (__init_options.add_bse_room) {
		if ((any_err = this->room_manager.add_room(this->bse_room_id, true)) != FFLY_SUCCESS) {
			firefly::system::io::printf(stderr, "ffly_client: room_manager, failed to add base room, ffly_errno: %d\n", ffly_errno);
			return any_err;
		}
	}

	if (__init_options.change_room) {
		if ((any_err = this->room_manager.change_room(this->bse_room_id)) != FFLY_SUCCESS) {
			firefly::system::io::printf(stderr, "ffly_client: room_manager, failed to change room, ffly_errno: %d\n", ffly_errno);
			return any_err;
		}
	}
# endif
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::ffly_client::de_init() {
	firefly::types::err_t any_err;
	if ((any_err = this->window.de_init()) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_client: failed to de_init window, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}
# ifdef ROOM_MANAGER
	if ((any_err = this->room_manager.de_init()) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_client: failed to de_init room manager, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}
# endif

# ifdef __WITH_OBJ_MANAGER
	if (this->obj_manager) {
		if ((any_err = this->obj_manager->de_init()) != FFLY_SUCCESS) {
			firefly::system::io::printf(stderr, "ffly_client: failed to de_init obj manager, ffly_errno: %d\n", ffly_errno);
			return any_err;
		}
	}
# endif

# ifdef __WITH_UNI_MANAGER
	if ((any_err = this->uni_manager.de_init()) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_client: failed to de_init uni manager, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}
# endif

	if ((any_err = this->asset_manager.de_init()) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_client: failed to de_init asset manager, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}

	if ((any_err = ffly_system::de_init()) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_client: failed to de_init ffly system, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}

	if ((any_err = ffly_graphics::de_init()) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_client: failed to de_init ffly graphics, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}

	this->cu_clean();
	return FFLY_SUCCESS;
}

void mdl::ffly_client::shutdown() {
	ffly_client::to_shutdown = true;
}
/*
bool mdl::ffly_client::poll_event(firefly::system::event& __event) {
	if (ffly_client::to_shutdown) return false;
	static bool inited = false;

	if (!inited) {
		this->event = &__event;
		inited = true;
	}*/
/*
	if (!__event.queue_empty()) {
		__event.event_type = __event.event_queue.front().event_type;
		switch (__event.event_type) {
			case firefly::system::event::KEY_PRESSED:
				__event.key_code = __event.event_queue.front().key_code;
			break;

			case firefly::system::event::KEY_RELEASED:
				__event.key_code = __event.event_queue.front().key_code;
			break;
		}

		__event.data = __event.event_queue.front().data;
		__event.queue_rm();
		return true;
	} else
		__event.event_type = firefly::system::event::NULL_EVENT;
*/
//	return false;
//}

mdl::firefly::types::err_t mdl::ffly_client::forward_events() {
	firefly::types::err_t any_err;
# ifdef ROOM_MANAGER
	if (!ffly_smem_buff_full(firefly::gui_btn_ev_dbuff)) {
		firefly::types::btn_event_t btn_event;
		uint_t event_type;
		if (this->room_manager.btn_event_poll(btn_event, event_type)) {
			ffly_smem_buff_push(firefly::gui_btn_ev_dbuff, &btn_event);
			void *btn_ev_dptr = NULL;
			ffly_smem_buff_ptr(firefly::gui_btn_ev_dbuff, &btn_ev_dptr);

			if (firefly::system::event::event_add((firefly::types::event_desc_t){.event_id = firefly::system::event_id::gui_btn, .event_type = event_type}, btn_ev_dptr) != FFLY_SUCCESS) {
				ffly_smem_buff_pop(firefly::gui_btn_ev_dbuff, NULL);
				if (ffly_errno != FF_ERR_CEBF) ffly_client::to_shutdown = true;
			}
		}
	} else
		firefly::system::io::printf(stdout, "ffly_client: info, btn event smem buff is full.\n");
# endif
	if (!ffly_smem_buff_full(firefly::wd_ev_dbuff)) {
		uint_t event_data;
		firefly::types::event_desc_t event_desc;
		if ((any_err = this->window.nxt_event(event_desc, event_data)) != FFLY_SUCCESS) {
			if (ffly_errno != FF_ERR_WDEQE) return any_err;
		} else if (ffly_errno == FF_ERR_WDEQE) ffly_errno = FF_ERR_NULL;

		if (ffly_errno != FF_ERR_WDEQE) {
			this->window.event_pop();

			void *wd_ev_dptr = NULL;
			ffly_smem_buff_push(firefly::wd_ev_dbuff, &event_data);
			ffly_smem_buff_ptr(firefly::wd_ev_dbuff, &wd_ev_dptr);
			firefly::system::io::printf(stdout, "wd event added: data: %d - id: %d, type: %d\n", event_data, event_desc.event_id, event_desc.event_type);
			if (firefly::system::event::event_add(event_desc, wd_ev_dptr) != FFLY_SUCCESS) {
				ffly_smem_buff_pop(firefly::wd_ev_dbuff, NULL);
				if (ffly_errno != FF_ERR_CEBF) ffly_client::to_shutdown = true;
			}
		}
	} else
		firefly::system::io::printf(stdout, "ffly_client: info, wd event smem buff is full.\n");

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::ffly_client::begin(char const * __frame_title, void (* __extern_loop)(boost::int8_t, portal_t *, void *), void *__this) {
	firefly::system::io::printf(stdout, "wx: %d, wy: %d\n", this->wd_xa_len, this->wd_ya_len);
	firefly::types::err_t any_err;
	if ((any_err = window.init(this->wd_xa_len, this->wd_ya_len, __frame_title)) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_client: failed to init window, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}

	if (window.begin() != FFLY_SUCCESS) return FFLY_FAILURE;

	window.wd_handle.set_fps_mark(60);

	while(!window.wd_handle.is_wd_flag(WD_OPEN)) {
		if (ffly_client::to_shutdown) return 0;
	}

	firefly::types::tpv_t begin = firefly::system::time::get_ms_tp();
	firefly::types::tpv_t end;
	if ((any_err = this->portal.init(this)) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_client: failed to init portal, ffly_errno: %d\n", ffly_errno);
		return FFLY_FAILURE;
	}
//	this->portal.pixbuff = window.get_pixbuff();//_x11_window.pixbuff;
//	this->portal._this = this;
	int sock;

# ifdef ROOM_MANAGER
	if (this->init_options.add_bse_layer)
		this->room_manager.set_pixbuff(this->layer.get_layer_pixmap(this->bse_layer_id));
	else
		this->room_manager.set_pixbuff(this->window.get_pixbuff());
# endif
	do {
		if (window.wd_handle.is_wd_flag(WD_CLOSED)) {
			firefly::system::io::printf(stdout, "window has been closed.\n");
			ffly_client::to_shutdown = true;
		}

		if (!window.wd_handle.is_wd_flag(WD_WAITING)) continue;
		if (window.wd_handle.is_wd_flag(WD_DONE_DRAW)) continue;
		this->window.clear_pixbuff();

		this->forward_events();

		client_info.key_code = window.wd_handle.key_code;

		if (this->server_connected) {
			if (this->send_client_info() == FFLY_FAILURE) break;
			if (this->recv_cam_frame() == FFLY_FAILURE) break;
		}
/*
		if (this->event != nullptr) {
			if (this->window.wd_handle.key_press) {
				firefly::system::event::event_t event;

				event.key_code = this->window.wd_handle.key_code;
				event.event_type = firefly::system::event::KEY_PRESSED;

				this->event->queue_add(event);
			} else if (!this->window.wd_handle.key_press && this->event->last_event == firefly::system::event::KEY_PRESSED) {
				firefly::system::event::event_t event;

				event.key_code = this->window.wd_handle.key_code;
				event.event_type = firefly::system::event::KEY_RELEASED;

				this->event->queue_add(event);
			}
		}
*/
		__extern_loop(0, &this->portal, __this);

		if (this->server_ipaddr != nullptr && !this->server_connected) {
			if (this->connect_to_server(sock) == -1) break;
		}

		skip:

# ifdef ROOM_MANAGER
		this->room_manager.manage();
# endif
		this->layer.draw_layers(window.get_pixbuff(), this->wd_xa_len, this->wd_ya_len);

# ifdef __WITH_UNI_MANAGER
		//this->uni_manager.draw_chunk(0, 0, 0, this->uni_manager._chunk_manager->coords_to_id(0,0,0), window.get_pixbuff(), this->wd_xa_len, this->wd_ya_len);
# endif

# ifdef __WITH_OBJ_MANAGER
		this->manage_objs();
		this->entity_manager.manage();
# endif
		window.wd_handle.add_wd_flag(WD_DONE_DRAW);
		window.wd_handle.rm_wd_flag(WD_WAITING);

		end = firefly::system::time::get_ms_tp();

		uint_t static frame_c{};
		firefly::types::tpv_t dur = end-begin;
		if (dur >= 1000) {
			begin = firefly::system::time::get_ms_tp();
			this->curr_fps = frame_c;
			frame_c = 0;
		} else frame_c++;

	} while (!ffly_client::to_shutdown);

	firefly::system::io::printf(stdout, "ffly_client: closed.\n");

	this->de_init();

	return 0;
}
