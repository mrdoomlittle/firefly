# include "obj_manager.hpp"
mdl::uint_t mdl::firefly::obj_manager::add(types::coords_t __coords, types::shape_info_t __shape_info, uint_t __xaxis_len, uint_t __yaxis_len, uint_t __zaxis_len) {
	types::obj_info_t obj_info;
	obj_info.xaxis_len = __xaxis_len;
	obj_info.yaxis_len = __yaxis_len;
	obj_info.zaxis_len = __zaxis_len;
	obj_info.coords = __coords;
	obj_info.velocity_enabled = true;
	obj_info.velocity.xaxis = 55;
	obj_info.velocity.yaxis = 55;
	obj_info.velocity.zaxis = 0;
	obj_info.velocity.direction.xaxis = 1;
	obj_info.velocity.direction.yaxis = 1;
	obj_info.velocity.direction.zaxis = 0;

	printf("x: %d, y:%d, z: %d\n", __coords.xaxis, __coords.yaxis, __coords.zaxis);

	obj_info.def_colour = {0, 0, 0, 0};

	uint_t obj_id = 0;
	if (!this-> unused_ids.empty())
	{
		std::set<uint_t>::iterator itor = this-> unused_ids.end();
		--itor;

		obj_id = *itor;

		this-> unused_ids.erase(itor);
	} else {
		obj_id = this-> obj_index.size();
		this-> obj_index.resize(this-> obj_index.size() + 1);
	}

	// for 2d
	obj_info.bound_collision = (bool *)memory::mem_alloc(4);
	memset(obj_info.bound_collision, false, 4);

	obj_info.collision.edge = (bool *)memory::mem_alloc(obj_info.edges);
	memset(obj_info.collision.edge, false, obj_info.edges);

	obj_info.collision.face = (bool *)memory::mem_alloc(obj_info.faces);
	memset(obj_info.collision.face, false, obj_info.faces);

	obj_info.edge_coords = (types::coords_t **)memory::mem_alloc(obj_info.edges * sizeof(types::coords_t *));
	for (std::size_t o = 0; o != obj_info.edges; o ++) {
		obj_info.edge_coords[o] = (types::coords_t *)memory::mem_alloc(2 * sizeof(types::coords_t));
		obj_info.edge_coords[o][0] = __shape_info.edge_coords[o][0];
		obj_info.edge_coords[o][1] = __shape_info.edge_coords[o][1];
	}

	obj_info.face_coords = (types::coords_t **)memory::mem_alloc(obj_info.faces * sizeof(types::coords_t *));
	for (std::size_t o = 0; o != obj_info.faces; o ++) {
		obj_info.face_coords[o] = (types::coords_t *)memory::mem_alloc(2 * sizeof(types::coords_t));
		obj_info.face_coords[o][0] = __shape_info.face_coords[o][0];
		obj_info.face_coords[o][1] = __shape_info.face_coords[o][1];
	}

	this-> obj_index[obj_id].first = obj_info;
	this-> obj_index[obj_id].second = (types::pixmap_t)memory::mem_alloc((__xaxis_len * __yaxis_len * __zaxis_len) * 4);
	memset(this-> obj_index[obj_id].second, 200, (__xaxis_len * __yaxis_len * __zaxis_len) * 4);

	this-> obj_count ++;
	return obj_id;
}

void mdl::firefly::obj_manager::del(uint_t __obj_id) {
	types::obj_info_t& obj_info = this-> obj_index[__obj_id].first;

	memory::mem_free(obj_info.collision.edge);
	memory::mem_free(obj_info.collision.face);
	memory::mem_free(obj_info.edge_coords);
	memory::mem_free(obj_info.face_coords);

	memory::mem_free(this-> obj_index[__obj_id].second);
	this-> unused_ids.insert(__obj_id);
	this-> obj_count --;
}

void mdl::firefly::obj_manager::handle_objs(thr_config_t *thread_config) {
	uint_t obj_id = thread_config-> offset;
	printf("threas starte.\n");
	system::stop_watch velos_timer[BLOCK_SIZE][3];

	gravy_manager gravity_mana;
	gravity_mana.init(BLOCK_SIZE);

	for (std::size_t o = 0; o != BLOCK_SIZE; o ++) {
		velos_timer[o][0].begin();
		velos_timer[o][1].begin();
		velos_timer[o][2].begin();
	}

	do {
		if (this-> waiting_for_thr && thread_config-> id) {
			while(this-> waiting_for_thr) {
				this-> thread_waiting = true;
			}
		}

		if (this-> unused_ids.find(obj_id) == this-> unused_ids.end()) {

		types::obj_info_t& obj_info = this-> obj_index[obj_id].first;
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
						flip_direction(obj_info.velocity.direction.xaxis);
					break;
					case 1:
						//obj_info.velocity.direction.xaxis = -1;
						flip_direction(obj_info.velocity.direction.xaxis);
					break;
					case 0:
						//obj_info.velocity.direction.yaxis = 0;
						flip_direction(obj_info.velocity.direction.yaxis);
					break;
					case 2:
						//obj_info.velocity.direction.yaxis = 0;
						flip_direction(obj_info.velocity.direction.yaxis);
					break;
				}
				obj_info.bound_collision[o] = false;
			}
		}

		if (obj_info.velocity_enabled) {
			velos_timer[obj_id][0].time_point();
			if (velos_timer[obj_id][0].get_delta<std::chrono::nanoseconds>() >= (1000000000 / obj_info.velocity.xaxis)) {
				if (obj_info.velocity.direction.xaxis == 1)
					this-> push_xaxis(obj_id, 1);
				else if (obj_info.velocity.direction.xaxis == -1)
					this-> pull_xaxis(obj_id, 1);
				velos_timer[obj_id][0].begin();
			}

			velos_timer[obj_id][1].time_point();
			if (velos_timer[obj_id][1].get_delta<std::chrono::nanoseconds>() >= (1000000000 / obj_info.velocity.yaxis)) {
				if (obj_info.velocity.direction.yaxis == 1)
					this-> push_yaxis(obj_id, 1);
				else if (obj_info.velocity.direction.yaxis == -1)
					this-> pull_yaxis(obj_id, 1);
				velos_timer[obj_id][1].begin();
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
	printf("thread dead..\n");
}

// for now until i can make draw_pixmap thread safe i will be using this
void mdl::firefly::obj_manager::draw_objs() {
	if (this-> obj_count == 0) {
		return;
	}

	uint_t obj_id = 0;
	while (obj_id != this-> obj_count) {
		types::obj_info_t obj_info = this-> obj_index[obj_id].first;
		types::pixmap_t obj_pixmap = this-> obj_index[obj_id].second;

		if (graphics::draw_pixmap(obj_info.coords.xaxis, obj_info.coords.yaxis, this-> pixbuff, this-> pb_xlen, this-> pb_ylen, obj_pixmap, obj_info.xaxis_len, obj_info.yaxis_len) == FFLY_FAILURE) {
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