# ifndef __ffly__location__h
# define __ffly__location__h

# include "ffint.h"
enum {
	_ff_loc_mem_alloc,
	_ff_loc_mem_free,
	_ff_loc_mem_realloc,
	_ff_loc_stores_connect,
	_ff_loc_stores_login,
	_ff_loc_stores_logout,
	_ff_loc_stores_disconnect,
	_ff_loc_db_ctr,
	_ff_loc_net_creat,
	_ff_loc_net_connect,
	_ff_loc_db_ctr_login
};

void ff_location_list(void);
void ff_location_show(void);
void ff_location_push(ff_uint_t);
void ff_location_pop(void);
# endif /*__ffly__location__h*/
