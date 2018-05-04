# ifndef __ffly__duct__h
# define __ffly__duct__h
# include "ffint.h"
# include "types.h"
ff_err_t ff_duct_open(ff_u8_t);
void ff_duct_close();
void ff_duct_connect();
void ff_duct_listen();
ff_i8_t ff_duct_serve();

void ff_duct_get_frame(void*, ff_uint_t, ff_uint_t, ff_u8_t);
void ff_duct_exit();
void ff_duct_done();
# endif /*__ffly__duct__h*/
