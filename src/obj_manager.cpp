# include "obj_manager.hpp"

mdl::uint_t mdl::firefly::obj_manager::add(uint_t __xlen, uint_t __ylen, uint_t __zlen, uint_t __xaxis, uint_t __yaxis, uint_t __zaxis) {
	types::obj_info_t obj_info;

	obj_info.xaxis_len = __xlen;
	obj_info.yaxis_len = __ylen;
	obj_info.zaxis_len = __zlen;
	obj_info.xaxis = __xaxis;
	obj_info.yaxis = __yaxis;
	obj_info.zaxis = __zaxis;
	obj_info.def_colour.r = 0;
	obj_info.def_colour.g = 0;
	obj_info.def_colour.b = 0;
	obj_info.def_colour.a = 255;
//		.xaxis_len = __xlen,
//		.yaxis_len = __ylen,
//		.zaxis_len = __zlen
//		.def_colour.r = 0,
//		.def_colour.g = 0,
//		.def_colour.b = 0,
//		.def_colour.a = 0
//	};
	uint_t obj_id = this-> obj_index.size();
	this-> obj_index.resize(this-> obj_index.size() + 1);

	this-> obj_index[obj_id].first = obj_info;
	types::pixmap_t pm = (types::pixmap_t)malloc((__xlen * __ylen * __zlen) * 4);
	memset(pm, 200, (__xlen * __ylen * __zlen) * 4);
	this-> obj_index[obj_id].second = pm;
	this-> obj_count ++;
	return obj_id;
}

void mdl::firefly::obj_manager::handle_objs(thr_config_t *thread_config) {
	uint_t obj_id = thread_config-> offset;
	printf("threas starte.\n");
	system::stop_watch stop_watch;
	stop_watch.begin();
	do {
		if (this-> waiting_for_thr && thread_config-> id) {
			while(this-> waiting_for_thr) {
				this-> thread_waiting = true;
			}
		}

		types::obj_info_t obj_info = this-> obj_index[obj_id].first;
		types::pixmap_t obj_pixmap = this-> obj_index[obj_id].second;

		if (obj_pixmap == nullptr || this-> pixbuff == nullptr) {
			fprintf(stderr, "obj pixmap is null.\n");
			break;
		}

		if (obj_info.skip_handle) {
			goto end;
		}

		stop_watch.time_point();
		if (stop_watch.get_delta<std::chrono::milliseconds>() >= (1000 / obj_info.gravity_speed)) {
			if (obj_info.gravity_enabled) {
				this-> push_yaxis(obj_id, 1);
			}
			stop_watch.begin();
		}

		end:
		if (obj_id >= (thread_config-> offset + thread_config-> amount) - 1)
			obj_id = thread_config-> offset;
		else obj_id ++;
	} while(1);
	printf("thread dead..\n");
}

void mdl::firefly::obj_manager::draw_objs() {
	if (this-> obj_count == 0) {
		return;
	}

	uint_t obj_id = 0;
	while (obj_id != this-> obj_count) {
		types::obj_info_t obj_info = this-> obj_index[obj_id].first;
		types::pixmap_t obj_pixmap = this-> obj_index[obj_id].second;

		if (graphics::draw_pixmap(obj_info.xaxis, obj_info.yaxis, this-> pixbuff, this-> pb_xlen, this-> pb_ylen, obj_pixmap, obj_info.xaxis_len, obj_info.yaxis_len) == FFLY_FAILURE) {
			fprintf(stderr, "failed to draw pixmap of object.\n");
			return;
		}

		obj_id ++;
	}
}

boost::int8_t mdl::firefly::obj_manager::manage() {
	static bool fist_bit = false;
	if (this-> obj_count > 0 && !fist_bit && this-> obj_count < BLOCK_SIZE) {
		uint_t thread_id = this-> thread_index.size();
		this-> thread_index.resize(this-> thread_index.size() + 1);

		thr_config_t *thread_config = (thr_config_t *)malloc(sizeof(thr_config_t));
		thread_config-> amount = this-> obj_count;
		thread_config-> offset = this-> thr_offset;
		thread_config-> id = thread_id;

		this-> thread_index[thread_id] = thread_config;

		//boost::thread *th = nullptr;
		boost::thread(boost::bind(&obj_manager::handle_objs, this, thread_config));

		fist_bit = true;
	}

	//if (this-> obj_count => BLOCK_SIZE && i )
	this-> draw_objs();

	return FFLY_SUCCESS;
}
