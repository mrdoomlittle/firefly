# ifndef __ffly__db__client__h
# define __ffly__db__client__h
# define __only_struc
# include "connect.h"
# include "../net.h"
# include "../types.h"
# include "../ffint.h"
typedef struct ff_db_cl {
	struct ff_db_conn conn;
	ff_tid_t thread;
	ff_uint_t id;
} *ff_db_clp;

ff_uint_t ff_db_client(FF_SOCKET*, void*);
void ff_db_client_destory(ff_uint_t);
# endif /*__ffly__db__client__h*/
