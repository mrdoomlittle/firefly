# include "flip_dir.hpp"
void mdl::firefly::flip_dir(std::atomic<boost::int8_t>& __dir) {
	switch(__dir) {
		case 1:
			__dir -= 2;
		break;
		case -1:
			__dir += 2;
		break;
	}
}
