# include "errno.h"
# ifndef _err
#	define _err(__r)(__r != FFLY_SUCCESS)
# endif
# ifndef _ok
#	define _ok(__r)(__r == FFLY_SUCCESS)
# endif
