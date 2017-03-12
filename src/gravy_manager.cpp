# include "gravy_manager.hpp"
boost::int8_t mdl::firefly::gravy_manager::init(uint_t __obj_c) {
	for (std::size_t o = 0; o != __obj_c; o ++) {
		this-> obj_timer[o].begin();
	}
	this-> obj_c = __obj_c;
	return FFLY_SUCCESS;
}

boost::int8_t mdl::firefly::gravy_manager::manage(obj_manager *__obj_mana_ptr, uint_t __obj_id, uint_t __gravity_speed, bool __gravity_enabled) {
	if (!__gravity_enabled) return FFLY_SUCCESS;
	this-> obj_timer[__obj_id].time_point();
	if (this-> obj_timer[__obj_id].get_delta<std::chrono::nanoseconds>() >= (1000000000 / __gravity_speed)) {
		__obj_mana_ptr-> push_yaxis(__obj_id, 1);
		this-> obj_timer[__obj_id].begin();
	}

	return FFLY_SUCCESS;
}
