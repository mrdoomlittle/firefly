# ifndef __ffly__system__hpp
# define __ffly__system__hpp
# include "system/io.h"
# include "types/err_t.h"
# include "system/errno.h"
# include "system/event.h"
# include "ffly_system.h"
# include "linux/types.h"
__linux_pid_t ffly_getpid();
namespace mdl {
namespace ffly_system {
firefly::types::err_t init() {
    ff_set_ppid(ffly_getppid());
    ff_setpid();
	ffly_io_init();
    ffly_system_init();
	ffly_queue_init(&ffly_event_queue, sizeof(ffly_event_t*));
	return FFLY_SUCCESS;
}

firefly::types::err_t de_init() {
	ffly_queue_de_init(&ffly_event_queue);
    ffly_system_de_init();
	ffly_io_closeup();
	return FFLY_SUCCESS;
}
}
}
# endif /*__ffly__system__hpp*/
