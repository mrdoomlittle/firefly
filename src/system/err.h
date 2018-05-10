# include "errno.h"
# include "../types.h"
// rename to error.h

# ifndef __ffly__err__h
# define __ffly__err__h
ff_err_t extern ffly_errval;
# endif

# ifndef ffly_errmsg
void _ffly_errmsg(char const*, int unsigned, char const*, ...);
# define ffly_errmsg(...) _ffly_errmsg(__FILE__, __LINE__, __VA_ARGS__)
# endif
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
# ifndef FF_ERR
#	define FF_ERR ff_err_t
# endif
# ifndef _ret
#	define _ret return err
# endif
