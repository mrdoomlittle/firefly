# ifndef __firefly__hpp
# define __firefly__hpp
# include "types/err_t.h"
# include "types/bool_t.h"
# ifdef __ffly_client
#	include "system/event.h"
#	include "system/event_field.h"
# endif
# include "act.h"
ffly_act_gid_t extern act_gid_de_init;
ffly_act_gid_t extern act_gid_cleanup;
namespace mdl {
# ifdef __ffly_client
class ffly_client;
# endif
namespace firefly {
types::err_t init();
types::err_t de_init();

# ifdef __ffly_client
types::bool_t poll_event(types::event_t*& __event);
void free_event(ffly_client *__ffc, types::event_t* __event);
# endif
}
}

# endif /*__firefly__hpp*/
