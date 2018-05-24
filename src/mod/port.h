# ifndef __ffly__mod__port__h
# define __ffly__mod__port__h
# include "../ffint.h"
ff_uint_t ffmod_port_shmid(void);
void ffmod_port(void);
void ffmod_port_open(ff_uint_t);
void ffmod_port_close(void);
ff_uint_t ffmod_portno(void);
# endif /*__ffly__mod__port__h*/
