# include "obj_manager.hpp"
void mdl::firefly::obj_manager::add_to_gravy_pool(uint_t __obj_id, uint_t __other_id) {
	uint_t th_id = __obj_id / BLOCK_SIZE;
	if (th_id % 1) return;
	gravy_queue_t queue_info;
	queue_info.command = 0;
	queue_info.obj_id = __obj_id;
	queue_info.block_id = th_id;
	queue_info.add_to_pool = __other_id;
	this-> gravy_queue.push(queue_info);

	//this-> thread_index[th_id]-> gravity_manager.add_to_pool(__obj_id, __other_id);
}

mdl::uint_t mdl::firefly::obj_manager::add(types::coords_t<> __coords, types::shape_info_t __shape_info, uint_t __xaxis_len, uint_t __yaxis_len, uint_t __zaxis_len, boost::int8_t& __any_error, types::pixmap_t __pixmap) {
	uint_t obj_id = 0;
	if (!this-> unused_ids.empty()) {
		std::set<uint_t>::iterator itor = this-> unused_ids.end();
		--itor;

		obj_id = *itor;
		this-> unused_ids.erase(itor);
	} else {
		obj_id = this-> obj_index.size();
		this-> obj_index.resize(this-> obj_index.size() + 1);
	}

	this-> obj_index[obj_id].first = (types::obj_info_t *)memory::mem_alloc(sizeof(types::obj_info_t));
	if (this-> obj_index[obj_id].first == NULL) {
		fprintf(stderr, "obj_manage: failed to alloc memory for obj_info_t, errno: %d\n", errno);
		__any_error = FFLY_FAILURE;
		return 0;
	}

	types::obj_info_t& obj_info = *this-> obj_index[obj_id].first;

	obj_info.xaxis_len = __xaxis_len;
	obj_info.yaxis_len = __yaxis_len;
	obj_info.zaxis_len = __zaxis_len;
	obj_info.gravity_speed = 5;
	obj_info.skip_handle = false;
	obj_info.coords.xaxis = __coords.xaxis;
	obj_info.coords.yaxis = __coords.yaxis;
	obj_info.coords.zaxis = __coords.zaxis;

	obj_info.velocity_enabled = true;
	obj_info.velocity.xaxis = 0;
	obj_info.velocity.yaxis = 0;
	obj_info.velocity.zaxis = 0;
	obj_info.velocity.direction.xaxis = FFLY_DIR_NONE;
	obj_info.velocity.direction.yaxis = FFLY_DIR_NONE;
	obj_info.velocity.direction.zaxis = FFLY_DIR_NONE;
	obj_info.edges = 0;
	obj_info.faces = 0;
	obj_info.vertices = 0;
	obj_info.mass = 212;

	printf("x: %d, y:%d, z: %d\n", __coords.xaxis, __coords.yaxis, __coords.zaxis);

	obj_info.def_colour = {109, 58, 59, 244};

	// for 2d
	obj_info.bound_collision = (bool *)memory::mem_alloc(4);
	if (obj_info.bound_collision == NULL) {
		fprintf(stderr, "obj_manager: failed to alloc memory for 'bound_collision', errno: %d\n", errno);
		__any_error = FFLY_FAILURE;
		goto clean_up;
	}

	memset(obj_info.bound_collision, false, 4);

	obj_info.collision.edge = (bool *)memory::mem_alloc(obj_info.edges);
	if (obj_info.collision.edge == NULL) {
		fprintf(stderr, "obj_manager: failed to alloc memory for 'collision.edge', errno: %d\n", errno);
		__any_error = FFLY_FAILURE;
		goto clean_up;
	}

	memset(obj_info.collision.edge, false, obj_info.edges);

	obj_info.collision.face = (bool *)memory::mem_alloc(obj_info.faces);
	if (obj_info.collision.face == NULL) {
		fprintf(stderr, "obj_manager: failed to alloc memory for 'collision.face', errno: %d\n", errno);
		__any_error = FFLY_FAILURE;
		goto clean_up;
	}

	memset(obj_info.collision.face, false, obj_info.faces);

	obj_info.collision.vertice = (bool *)memory::mem_alloc(obj_info.vertices);
	if (obj_info.collision.vertice == NULL) {
		fprintf(stderr, "obj_manager: failed to alloc memory for 'collision.vertice', errno: %d\n", errno);
		__any_error = FFLY_FAILURE;
		goto clean_up;
	}

	memset(obj_info.collision.vertice, false, obj_info.vertices);

	obj_info.edge_coords = (types::coords_t<> **)memory::mem_alloc(obj_info.edges * sizeof(types::coords_t<> *));
	if (obj_info.edge_coords == NULL) {
		fprintf(stderr, "obj_manager: failed to alloc memory for 'edge_coords', errno: %d\n", errno);
		__any_error = FFLY_FAILURE;
		goto clean_up;
	}

	for (std::size_t o = 0; o != obj_info.edges; o ++) {
		obj_info.face_coords[o] = (types::coords_t<> *)memory::mem_alloc(2 * sizeof(types::coords_t<>));
		if (obj_info.face_coords[o] == NULL) {
			fprintf(stderr, "obj_manager: failed to alloc memory for 'edge_coords', errno: %d\n", errno);
			__any_error = FFLY_FAILURE;
			goto clean_up;
		}

		obj_info.face_coords[o][0] = __shape_info.face_coords[o][0];
		obj_info.face_coords[o][1] = __shape_info.face_coords[o][1];
	}

	obj_info.face_coords = (types::coords_t<> **)memory::mem_alloc(obj_info.faces * sizeof(types::coords_t<> *));
	if (obj_info.face_coords == NULL) {
		fprintf(stderr, "obj_manager: failed to alloc memory for 'face_coords', errno: %d\n", errno);
		__any_error = FFLY_FAILURE;
		goto clean_up;
	}

	for (std::size_t o = 0; o != obj_info.edges; o ++) {
		obj_info.edge_coords[o] = (types::coords_t<> *)memory::mem_alloc(2 * sizeof(types::coords_t<>));
		if (obj_info.edge_coords[o] == NULL) {
			fprintf(stderr, "obj_manager: failed to alloc memory for 'face_coords', errno: %d\n", errno);
			__any_error = FFLY_FAILURE;
			goto clean_up;
		}

		obj_info.edge_coords[o][0] = __shape_info.edge_coords[o][0];
		obj_info.edge_coords[o][1] = __shape_info.edge_coords[o][1];
	}

	obj_info.vertice_coords = (types::coords_t<> **)memory::mem_alloc(obj_info.vertices * sizeof(types::coords_t<> *));
	if (obj_info.vertice_coords == NULL) {
		fprintf(stderr, "obj_manager: failed to alloc memory for 'vertice_coords', errno: %d\n", errno);
		__any_error = FFLY_FAILURE;
		goto clean_up;
	}

	for (std::size_t o = 0; o != obj_info.vertices; o ++) {
		obj_info.vertice_coords[o] = (types::coords_t<> *)memory::mem_alloc(2 * sizeof(types::coords_t<>));
		if (obj_info.vertice_coords[o] == NULL) {
			fprintf(stderr, "obj_manager: failed to alloc memory for 'vertice_coords', errno: %d\n", errno);
			__any_error = FFLY_FAILURE;
			goto clean_up;
		}

		obj_info.vertice_coords[o][0] = __shape_info.vertice_coords[o][0];
		obj_info.vertice_coords[o][1] = __shape_info.vertice_coords[o][1];
	}

	if (__pixmap == nullptr) {
		this-> obj_index[obj_id].second = (types::pixmap_t)memory::mem_alloc((__xaxis_len * __yaxis_len * __zaxis_len) * 4);
		if (this-> obj_index[obj_id].second == NULL) {
			fprintf(stderr, "obj_manager: failed to alloc memory for pixmap, errno: %d\n", errno);
			__any_error = FFLY_FAILURE;
			goto clean_up;
		}

		if (graphics::fill_pixmap(this-> obj_index[obj_id].second, __xaxis_len, __yaxis_len, obj_info.def_colour) != FFLY_SUCCESS) {
			fprintf(stderr, "obj_manager: gpu failed to fill pixmap, using the cpu instead.\n");
			for (uint_t pix_point = 0; pix_point != (__xaxis_len * __yaxis_len * __zaxis_len) * 4; pix_point += 4) {
				this-> obj_index[obj_id].second[pix_point] = obj_info.def_colour.r;
				this-> obj_index[obj_id].second[pix_point + 1] = obj_info.def_colour.g;
				this-> obj_index[obj_id].second[pix_point + 2] = obj_info.def_colour.b;
				this-> obj_index[obj_id].second[pix_point + 3] = obj_info.def_colour.a;
			}
		}
	} else {
		if (__pixmap == nullptr) {
			fprintf(stderr, "obj_manager: can't use pixmap provided as memory has not been allocated for it.\n");
			__any_error = FFLY_FAILURE;
			goto clean_up;
		}

		this-> obj_index[obj_id].second = __pixmap;
	}

	__any_error = FFLY_SUCCESS;
	this-> obj_count ++;
	return obj_id;

	clean_up:
	if (obj_info.bound_collision != NULL)
		memory::mem_free(obj_info.bound_collision);

	if (obj_info.collision.edge != NULL)
		memory::mem_free(obj_info.collision.edge);

	if (obj_info.collision.face != NULL)
		memory::mem_free(obj_info.collision.face);

	if (obj_info.collision.vertice != NULL)
		memory::mem_free(obj_info.collision.vertice);

	if (obj_info.edge_coords != NULL)
		memory::mem_free(obj_info.edge_coords);

	if (obj_info.face_coords != NULL)
		memory::mem_free(obj_info.face_coords);

	if (obj_info.vertice_coords != NULL)
		memory::mem_free(obj_info.vertice_coords);

	if (this-> obj_index[obj_id].first != NULL)
		memory::mem_free(this-> obj_index[obj_id].first);

	if (this-> obj_index[obj_id].second != NULL)
		memory::mem_free(this-> obj_index[obj_id].second);
}

void mdl::firefly::obj_manager::del(uint_t __obj_id) {
	types::obj_info_t& obj_info = *this-> obj_index[__obj_id].first;

	memory::mem_free(obj_info.bound_collision);
	memory::mem_free(obj_info.collision.edge);
	memory::mem_free(obj_info.collision.face);
	memory::mem_free(obj_info.collision.vertice);

	memory::mem_free(obj_info.edge_coords);
	memory::mem_free(obj_info.face_coords);
	memory::mem_free(obj_info.vertice_coords);
	memory::mem_free(this-> obj_index[__obj_id].first);
	memory::mem_free(this-> obj_index[__obj_id].second);
	this-> unused_ids.insert(__obj_id);
	this-> obj_count --;
}

boost::int8_t mdl::firefly::obj_manager::de_init() {
	this-> to_shutdown = true;
	printf("waiting for all obj thread to be killed.\n");
	while (this-> active_threads != 0) {}

	printf("%d threads where killed.\n", unsigned(this-> active_threads));

	for (uint_t obj_id = 0; obj_id != this-> obj_index.size(); obj_id ++) {
		if (this-> unused_ids.find(obj_id) != this-> unused_ids.end()) continue;
		this-> del(obj_id);
	}

	return FFLY_SUCCESS;
}

void mdl::firefly::obj_manager::handle_objs(thr_config_t *thread_config) {
	this-> active_threads ++;

	uint_t obj_id = thread_config-> offset;
	printf("threas starte.\n");
	system::stop_watch velos_timer[BLOCK_SIZE][3];

	gravy_manager gravity_mana;// = thread_config-> gravity_manager;
	gravity_mana.init(BLOCK_SIZE);

	while(!this-> gravy_queue.empty()) {
		if (this-> gravy_queue.front().block_id == thread_config-> id) {
			gravy_queue_t& element = this-> gravy_queue.front();
			switch(element.command) {
				case 0:
					gravity_mana.add_to_pool(element.obj_id, element.add_to_pool);
					this-> gravy_queue.pop();
				break;
			}
		}
	}

	for (std::size_t o = 0; o != BLOCK_SIZE; o ++) {
		velos_timer[o][0].begin();
		velos_timer[o][1].begin();
		velos_timer[o][2].begin();
	}

	system::stop_watch drag_timer;
	drag_timer.begin();

	do {
		if (this-> to_shutdown) break;
		if (this-> th_wait && this-> th_id_to_wait == thread_config-> id) {
			while(this-> th_wait) {
				this-> th_waiting = true;
			}
		}

		if (this-> unused_ids.find(obj_id) == this-> unused_ids.end()) {

		types::obj_info_t& obj_info = *this-> obj_index[obj_id].first;
		types::pixmap_t obj_pixmap = this-> obj_index[obj_id].second;

		uint_t obj_gr_id = (obj_id - thread_config-> offset);
		if (obj_pixmap == nullptr || this-> pixbuff == nullptr) {
			fprintf(stderr, "obj pixmap is null.\n");
			break;
		}

		if (obj_info.skip_handle) {
			goto end;
		}
/*
{
		std::size_t sec_obj_id = 0;
		while(sec_obj_id != this-> obj_index.size()) {
			if (sec_obj_id == obj_id) {
				sec_obj_id ++;
				continue;
			}

			types::obj_info_t sec_obj_info = this-> obj_index[sec_obj_id].first;

			types::coords_t obj_0 = {obj_info.xaxis, obj_info.yaxis};
			types::coords_t obj_1 = {sec_obj_info.xaxis, sec_obj_info.yaxis};
			uint_t dis = maths::cal_dist(obj_0, obj_1);

			double grav_force = round(((double)obj_info.mass * (double)sec_obj_info.mass) / (double)maths::squar(dis));

			printf("grav: %lf\n", grav_force);
			usleep(100000);

			sec_obj_id ++;
		}
}
*/
		drag_timer.time_point();
		if (drag_timer.get_delta<std::chrono::nanoseconds>() >= (1000000000 / 66)) {
			if (this-> xaxis_direction(obj_id) == FFLY_DIR_PUSH) {
				this-> rm_xaxis_velos(obj_id, 1);
			}

			if (this-> yaxis_direction(obj_id) == FFLY_DIR_PUSH) {
				this-> rm_yaxis_velos(obj_id, 1);
			}

			if (this-> xaxis_direction(obj_id) == FFLY_DIR_PULL) {
				this-> add_xaxis_velos(obj_id, 1);
			}

			if (this-> yaxis_direction(obj_id) == FFLY_DIR_PULL) {
				this-> add_yaxis_velos(obj_id, 1);
			}

			drag_timer.begin();
		}


		if (obj_info.coords.xaxis < obj_info.xaxis_bound[0] + 1)
			obj_info.bound_collision[3] = true;
		//else obj_info.bound_collision[3] = false;

		if ((obj_info.coords.xaxis + obj_info.xaxis_len) > obj_info.xaxis_bound[1] - 2)
			obj_info.bound_collision[1] = true;
		//else obj_info.bound_collision[1] = false;

		if (obj_info.coords.yaxis < obj_info.yaxis_bound[0] + 1)
			obj_info.bound_collision[0] = true;
		//else obj_info.bound_collision[0] = false;

		if ((obj_info.coords.yaxis + obj_info.yaxis_len) > obj_info.yaxis_bound[1] - 2)
			obj_info.bound_collision[2] = true;
		//else obj_info.bound_collision[2] = false;

		for (std::size_t o = 0; o != 4; o ++) {
			if(obj_info.bound_collision[o]) {
				switch(o) {
					case 3:
						//obj_info.velocity.direction.xaxis = 0;
						flip_dir(obj_info.velocity.direction.xaxis);
					break;
					case 1:
						//obj_info.velocity.direction.xaxis = -1;
						flip_dir(obj_info.velocity.direction.xaxis);
					break;
					case 0:
						//obj_info.velocity.direction.yaxis = 0;
						flip_dir(obj_info.velocity.direction.yaxis);
					break;
					case 2:
						//obj_info.velocity.direction.yaxis = 0;
						flip_dir(obj_info.velocity.direction.yaxis);
					break;
				}
				obj_info.bound_collision[o] = false;
			}
		}

		if (obj_info.velocity_enabled) {
			if (obj_info.velocity.xaxis != 0) {
			velos_timer[obj_id][0].time_point();
			if (velos_timer[obj_id][0].get_delta<std::chrono::nanoseconds>() >= (1000000000 / obj_info.velocity.xaxis)) {
				if (obj_info.velocity.direction.xaxis == 1)
					this-> push_xaxis(obj_id, 1);
				else if (obj_info.velocity.direction.xaxis == -1)
					this-> pull_xaxis(obj_id, 1);
				velos_timer[obj_id][0].begin();
			}
			}

			if (obj_info.velocity.yaxis != 0) {
			velos_timer[obj_id][1].time_point();
			if (velos_timer[obj_id][1].get_delta<std::chrono::nanoseconds>() >= (1000000000 / obj_info.velocity.yaxis)) {
				if (obj_info.velocity.direction.yaxis == 1)
					this-> push_yaxis(obj_id, 1);
				else if (obj_info.velocity.direction.yaxis == -1)
					this-> pull_yaxis(obj_id, 1);
				velos_timer[obj_id][1].begin();
			}
			}
/*
			velos_timer[obj_id][2].time_point();
			if (velos_timer[obj_id][2].get_delta<std::chrono::nanoseconds>() >= (1000000000 / obj_info.velocity.zaxis)) {
				if (obj_info.velocity.direction.zaxis == 1)
					this-> push_zaxis(obj_id, 1);
				else if (obj_info.velocity.direction.zaxis == -1)
					this-> pull_zaxis(obj_id, 1);
				velos_timer[obj_id][2].begin();
			}
*/
		}

		gravity_mana.manage(this, obj_id, obj_info.gravity_speed, obj_info.gravity_enabled);
}
		end:
		if (obj_id >= (thread_config-> offset + thread_config-> amount) - 1)
			obj_id = thread_config-> offset;
		else obj_id ++;
	} while(1);

	gravity_mana.de_init();

	this-> active_threads --;
	printf("thread dead..\n");
}

// for now until i can make draw_pixmap thread safe i will be using this
void mdl::firefly::obj_manager::draw_objs() {
	if (this-> to_shutdown) return;
	if (this-> obj_count == 0) {
		return;
	}

	uint_t obj_id = 0;
	while (obj_id != this-> obj_count) {
		if (this-> obj_index[obj_id].first == nullptr || this-> obj_index[obj_id].second == nullptr) { obj_id ++; continue; }

		types::obj_info_t& obj_info = *this-> obj_index[obj_id].first;
		types::pixmap_t obj_pixmap = this-> obj_index[obj_id].second;

		if (graphics::draw_pixmap(obj_info.coords.xaxis, obj_info.coords.yaxis, this-> pixbuff, this-> pb_xlen, this-> pb_ylen, obj_pixmap, obj_info.xaxis_len, obj_info.yaxis_len) == FFLY_FAILURE) {
			fprintf(stderr, "obj_manager: failed to draw pixmap for obj with gpu, going to use cpu instead.\n");
			graphics::cpu_draw_pixmap(obj_info.coords.xaxis, obj_info.coords.yaxis, this-> pixbuff, this-> pb_xlen, this-> pb_ylen, obj_pixmap, obj_info.xaxis_len, obj_info.yaxis_len);
		}

		obj_id ++;
	}
}

boost::int8_t mdl::firefly::obj_manager::manage() {
	if (this-> to_shutdown) return FFLY_NOP;
	static bool fist_bit = false;
	if (this-> obj_count > 0 && !fist_bit && this-> obj_count < BLOCK_SIZE) {
		uint_t thread_id = this-> thread_index.size();
		this-> thread_index.resize(this-> thread_index.size() + 1);

		thr_config_t *thread_config = (thr_config_t *)malloc(sizeof(thr_config_t));
		if (thread_config == NULL) {
			fprintf(stderr, "obj_manager: failed to alloc memory for 'thread_config', errno: %d\n", errno);
			this-> thread_index.resize(this-> thread_index.size() - 1);
			return FFLY_FAILURE;
		}

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
