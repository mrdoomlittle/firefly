# include "gravy_manager.hpp"
boost::int8_t mdl::firefly::gravy_manager::init(uint_t __obj_c) {
	for (std::size_t o = 0; o != __obj_c; o ++) {
		this-> obj_timer[o].begin();
	}
	this-> obj_c = __obj_c;
	return FFLY_SUCCESS;
}
boost::int8_t mdl::firefly::gravy_manager::de_init() {
	for (std::size_t o = 0; o != BLOCK_SIZE; o ++)
		this-> gravy_pool[o].clear();
}

# define GCONST 212//8129
boost::int8_t mdl::firefly::gravy_manager::manage(obj_manager *__obj_manager, uint_t __obj_id, uint_t __gravity_speed, bool __gravity_enabled) {
	if (!__gravity_enabled) return FFLY_SUCCESS;
	this-> obj_timer[__obj_id].time_point();
	if (this-> obj_timer[__obj_id].get_delta<std::chrono::nanoseconds>() >= (1000000000 / __gravity_speed)) {
		__obj_manager-> push_yaxis(__obj_id, 1);
		this-> obj_timer[__obj_id].begin();
	}

	std::size_t o = 0;
	while (o != this-> gravy_pool[__obj_id].size()) {
		pool_ele_t& pool_ele = this-> gravy_pool[__obj_id][o];
		if (!pool_ele.inited) {
			pool_ele.timer.begin();
			pool_ele.inited = true;
		}

		if (__obj_manager-> get_xaxis(__obj_id) == __obj_manager-> get_xaxis(pool_ele.obj_id)
		|| __obj_manager-> get_yaxis(__obj_id) == __obj_manager-> get_yaxis(pool_ele.obj_id)) {
			o++;
			continue;
		}

		uint_t dist = maths::cal_dist(__obj_manager-> get_coords(__obj_id), __obj_manager-> get_coords(pool_ele.obj_id));
		double gravity = (__obj_manager-> get_mass(__obj_id) * __obj_manager-> get_mass(pool_ele.obj_id)) / maths::squar_rt(dist);
		//printf("gravity: %d\n", gravity);
		pool_ele.timer.time_point();

		//uint_t pix_per_second = round((GCONST / gravity)/1);
		if (pool_ele.timer.get_delta<std::chrono::nanoseconds>() < ((GCONST / gravity) * 1000000000)) {o ++; continue;}
		printf("gravity speed: %lf\n", (GCONST / gravity));

		uint_t delta_x = 0, delta_y = 0;

		uint_t me_x = __obj_manager-> get_xaxis(__obj_id), other_x = __obj_manager-> get_xaxis(pool_ele.obj_id);

		if (me_x < other_x)
			delta_x = other_x - me_x;
		else
			delta_x = me_x - other_x;

		uint_t me_y = __obj_manager-> get_yaxis(__obj_id), other_y = __obj_manager-> get_yaxis(pool_ele.obj_id);

		if (me_y < other_y)
			delta_y = other_y - me_y;
		else
			delta_y = me_y - other_y;

		uint_t deltaerr = abs(delta_y / delta_x);
		uint_t error = deltaerr - 0.5;

		if (me_x < other_x) {
			if (__obj_manager-> xaxis_direction(__obj_id) != FFLY_DIR_PUSH) {
				if (__obj_manager-> get_xaxis_velos(__obj_id) != 0)
					__obj_manager-> rm_xaxis_velos(__obj_id, 1);
				else
					 __obj_manager-> xaxis_direction(__obj_id) = FFLY_DIR_PUSH;
			} else
				__obj_manager-> add_xaxis_velos(__obj_id, 1);
		} else {
			if (__obj_manager-> xaxis_direction(__obj_id) != FFLY_DIR_PULL) {
				if (__obj_manager-> get_xaxis_velos(__obj_id) != 0)
					__obj_manager-> rm_xaxis_velos(__obj_id, 1);
				else
					__obj_manager-> xaxis_direction(__obj_id) = FFLY_DIR_PULL;
			} else
				__obj_manager-> add_xaxis_velos(__obj_id, 1);
		}

		if (error + deltaerr > 0.5) {
			if (me_y < other_y) {
				if (__obj_manager-> yaxis_direction(__obj_id) != FFLY_DIR_PUSH) {
					if (__obj_manager-> get_yaxis_velos(__obj_id) != 0)
						__obj_manager-> rm_yaxis_velos(__obj_id, 1);
					else
						__obj_manager-> yaxis_direction(__obj_id) = FFLY_DIR_PUSH;
				} else
					__obj_manager-> add_yaxis_velos(__obj_id, 1);
			} else {
				if (__obj_manager-> yaxis_direction(__obj_id) != FFLY_DIR_PULL) {
					if (__obj_manager-> get_yaxis_velos(__obj_id) != 0)
						__obj_manager-> rm_yaxis_velos(__obj_id, 1);
					else
						__obj_manager-> yaxis_direction(__obj_id) = FFLY_DIR_PULL;
				} else
					__obj_manager-> add_yaxis_velos(__obj_id, 1);
			}
		}

		printf("velos [x: %d, y: %d]\n", __obj_manager-> get_xaxis_velos(__obj_id), __obj_manager-> get_yaxis_velos(__obj_id));

		pool_ele.inited = false;
		o ++;
	}

	return FFLY_SUCCESS;
}
