# ifndef __ffly__system__hpp
# define __ffly__system__hpp
# ifdef __WITH_TASK_HANDLE
#	include "system/task_handle.hpp"
# endif
# include "types/err_t.h"
# define _TH_THREADS 12
namespace mdl {
namespace ffly_system {
static firefly::types::err_t init() {
# ifdef __WITH_TASK_HANDLE
	if (firefly::system::_task_handle.init(_TH_THREADS) == FFLY_FAILURE)
		return FFLY_FAILURE;
# endif
	return FFLY_SUCCESS;
}

static firefly::types::err_t de_init() {
# ifdef __WITH_TASK_HANDLE
	if (firefly::system::_task_handle.de_init() == FFLY_FAILURE)
		return FFLY_FAILURE;
# endif
	return FFLY_SUCCESS;
}

}
}

# endif /*__ffly__system__hpp*/
