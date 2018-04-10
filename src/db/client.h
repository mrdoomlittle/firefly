# ifndef __ffly__db__client__h
# define __ffly__db__client__h
# define __only_struc
# include "connect.h"
# include "../net.h"
# include "../types/thread.h"
# include <mdlint.h>
typedef struct ff_db_cl {
	struct ff_db_conn conn;
	ffly_tid_t thread;
	mdl_uint_t id;
} *ff_db_clp;

mdl_uint_t ff_db_client(FF_SOCKET*, void*);
void ff_db_client_destory(mdl_uint_t);
# endif /*__ffly__db__client__h*/
