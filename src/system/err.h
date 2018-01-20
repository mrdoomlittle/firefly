# include "errno.h"
# ifndef _err
#	define _err(__r)((__r) != FFLY_SUCCESS)
# endif
# ifndef _ok
#	define _ok(__r)((__r) == FFLY_SUCCESS)
# endif
# ifndef reterr
#   define reterr return FFLY_FAILURE
# endif
# ifndef retok
#   define retok return FFLY_SUCCESS
# endif
