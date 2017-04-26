# include "firefly.hpp"

mdl::firefly::types::err_t mdl::firefly::init() {
	ffly_memory::init();
	ffly_system::init();
	ffly_graphics::init();
}

mdl::firefly::types::err_t mdl::firefly::de_init() {
	ffly_memory::de_init();
	ffly_system::de_init();
	ffly_graphics::de_init();
}

namespace mdl {
namespace firefly {
namespace gpu {
uint_t device_c = 0, mx_threads = 0, mx_blocks = 0;
}
}

namespace ffly_graphics {
bool inited = false;
}
}
