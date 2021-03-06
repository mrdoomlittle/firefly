# ifndef __ffly__duct__h
# define __ffly__duct__h
# include "ffint.h"
# include "types.h"
# include "event.h"
# include "system/event.h"

ff_err_t ff_duct_open(ff_u8_t);
void ff_duct_close();
void ff_duct_connect();
void ff_duct_listen();
ff_i8_t ff_duct_serve();

void ff_duct_get_frame(void*, ff_uint_t, ff_uint_t, ff_u8_t);
void ff_duct_exit();
void ff_duct_done();
void ff_duct_read(void*, ff_uint_t, ff_uint_t, ff_uint_t);
void ff_duct_write(void*, ff_uint_t, ff_uint_t, ff_uint_t);
ff_uint_t ff_duct_alloc(ff_uint_t);
void ff_duct_free(ff_uint_t);
void ff_duct_event(ff_eventp);
# endif /*__ffly__duct__h*/
