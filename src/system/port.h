# ifndef __ffly__port__h
# define __ffly__port__h
# include "../ffint.h"
# include "../types.h"

# define FF_PORT_CREAT 0x1
# define FF_PORT_SHMM 0x2
ff_uint_t ffly_port_get_shmid(ff_uint_t);
ff_err_t ffly_port_send(ff_u32_t, void*, ff_uint_t, ff_uint_t);
ff_err_t ffly_port_recv(ff_u32_t, void*, ff_uint_t, ff_uint_t);
ff_uint_t ffly_port(ff_uint_t, ff_u8_t, ff_uint_t, ff_err_t*);

ff_i8_t ffly_port_band(ff_uint_t, ff_u32_t);
void ffly_port_close(ff_uint_t);
# endif /*__ffly__port__h*/
