# include "firefly.hpp"
# include "types/event_t.h"
# include "ffly_def.h"
/*
struct mdl::firefly::_core_portal mdl::firefly::core_portal;

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
*/

mdl::firefly::types::bool_t mdl::firefly::poll_event(types::event_t& __event) {
	if (!mdl::firefly::system::pending_event()) return ffly_false;
	mdl::firefly::system::next_event(__event);
	return ffly_true;
}

void mdl::firefly::event_free(ffly_client *__ffc, types::event_t& __event) {
	if (__event.field == _ffly_ef_wd) {
		__ffc->free_wd_event(__event);
	}
}

/*
namespace mdl {
namespace firefly {
types::err_t init_core_portal(graphics::window *__window) {
	core_portal = (struct mdl::firefly::_core_portal) {
		pointer_coords:&__window->pointer_coords,
		wd_coords:&__window->wd_coords,
		wd_xa_len:&__window->wd_xaxis_len,
		wd_ya_len:&__window->wd_yaxis_len
	};
}

struct ffly_smem_buff_t *gui_btn_ev_dbuff = ffly_smem_buff_ninst(26, sizeof(types::btn_event_t));
struct ffly_smem_buff_t *wd_ev_dbuff = ffly_smem_buff_ninst(26, sizeof(uint_t));
types::bool_t poll_event(types::event_t& __event) {
	if (!(__event.event_data = system::event::nxt_event(__event.event_desc))) return false;
	system::event::event_pop();
	return true;
}

void pev_collapse(types::event_id_t __event_id) {
	switch(__event_id) {
		case system::event_id::gui_btn:
			ffly_smem_buff_pop(gui_btn_ev_dbuff, NULL);
		break;
		case system::event_id::key:
			ffly_smem_buff_pop(wd_ev_dbuff, NULL);
		break;
		case system::event_id::btn:
			ffly_smem_buff_pop(wd_ev_dbuff, NULL);
		break;
	}
}
*/
/*
namespace gpu {
uint_t device_c = 0, mx_threads = 0, mx_blocks = 0;
}
}

namespace ffly_graphics {
bool inited = false;
}
}*/

