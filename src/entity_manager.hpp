# ifndef __entity__manager__hpp
# define __entity__manager__hpp
# include "obj_manager.hpp"
namespace mdl {
namespace firefly {
class entity_manager {
	boost::int8_t init(obj_manager *__obj_manager) {
		this-> _obj_manager = __obj_manager;-
	}

	uint_t add_entity() {

	}

	private:
	obj_manager *_obj_manager = nullptr;
};
}
}

# endif /*__entity__manager__hpp*/
