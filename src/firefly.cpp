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
struct ffly_smem_buff_t *gui_btn_ev_dbuff = ffly_smem_buff_ninst(26, sizeof(types::btn_event_t));
struct ffly_smem_buff_t *wd_ev_dbuff = ffly_smem_buff_ninst(26, sizeof(uint_t));
bool poll_event(types::event_t& __event) {
	if (!(__event.event_data = system::event::nxt_event(__event.event_disc))) return false;
	system::event::event_pop();
	return true;
}

void pev_collapse(types::event_id_t __event_id) {
	switch(__event_id) {
		case system::GUI_BTN_EID:
			ffly_smem_buff_pop(gui_btn_ev_dbuff, NULL);
		break;
		case system::KEY_EID:
			ffly_smem_buff_pop(wd_ev_dbuff, NULL);
		break;
		case system::BTN_EID:
			ffly_smem_buff_pop(wd_ev_dbuff, NULL);
		break;
	}
}

namespace gpu {
uint_t device_c = 0, mx_threads = 0, mx_blocks = 0;
}
}

namespace ffly_graphics {
bool inited = false;
}
}
