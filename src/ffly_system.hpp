# ifndef __ffly__system__hpp
# define __ffly__system__hpp
/*
# ifdef __WITH_TASK_HANDLE
#	include "system/task_handle.hpp"
# endif
# ifdef __WITH_MEM_TRACKER
#	include "system/mem_tracker.h"
# endif
# include "types/err_t.h"
# define _TH_THREADS 12
namespace mdl {
namespace ffly_system {
firefly::types::err_t static init() {
# ifdef __WITH_TASK_HANDLE
	if (firefly::system::_task_handle.init(_TH_THREADS) == FFLY_FAILURE)
		return FFLY_FAILURE;
# endif
# ifdef __WITH_MEM_TRACKER
	ffly_mem_track_init(&__ffly_mem_track__);
# endif
	return FFLY_SUCCESS;
}

firefly::types::err_t static de_init() {
# ifdef __WITH_TASK_HANDLE
	if (firefly::system::_task_handle.de_init() == FFLY_FAILURE)
		return FFLY_FAILURE;
# endif
# ifdef __WITH_MEM_TRACKER
	ffly_mem_track_de_init(&__ffly_mem_track__);
# endif
	return FFLY_SUCCESS;
}

}
}
*/
# include "system/io.h"
# include "types/err_t.h"
# include "system/errno.h"
namespace mdl {
namespace ffly_system {
firefly::types::err_t init() {
	ffly_io_init();
	return FFLY_SUCCESS;
}

firefly::types::err_t de_init() {
	ffly_io_closeup();
	return FFLY_SUCCESS;
}
}
}
# endif /*__ffly__system__hpp*/
