# ifndef __firefly__hpp
# define __firefly__hpp
# include "types/err_t.h"
# include "ffly_graphics.hpp"
# include "ffly_system.hpp"
# include "ffly_memory.hpp"
# include "system/event.hpp"
# include "system/smem_buff.h"
# include "types/btn_event_t.hpp"
# include "types/event_id_t.hpp"
# include "types/event_t.hpp"
namespace mdl {
namespace firefly {
types::err_t init();
types::err_t de_init();
types::err_t de_init();
struct ffly_smem_buff_t extern *gui_btn_ev_dbuff;
struct ffly_smem_buff_t extern *wd_ev_dbuff;

bool poll_event(types::event_t& __event);
void pev_collapse(types::event_id_t __event_id);
}
}

# endif /*__firefly__hpp*/
