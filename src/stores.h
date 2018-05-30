# ifndef __ffly__stores__h
# define __ffly__stores__h
# include "ffint.h"
# include "types.h"
enum {
	_ff_stores_motd
};

void* ff_stores_get(ff_u8_t);
ff_err_t ff_stores_connect(void);
ff_err_t ff_stores_login(void);
ff_err_t ff_stores_logout(void);
ff_err_t ff_stores_disconnect(void);
# endif /*__ffly__stores__h*/
