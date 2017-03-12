# include "flip_direction.hpp"
void mdl::firefly::flip_direction(boost::int8_t& __direction) {
	switch(__direction) {
		case 1:
			__direction -= 2;
		break;
		case -1:
			__direction += 2;
		break;
	}
}
