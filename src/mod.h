# ifndef __ffly__mod__h
# define __ffly__mod__h
# include "ffint.h"
# define MOD_LISTEN 1
void ff_mod_load(char const*);
void ffmodldl(char const**);
void ff_mod_handle();
void ff_mod_port_open();
void ff_mod_port_close();
void ffly_mod();
void ff_mod_init();
void ff_mod_de_init();
# endif /*__ffly__mod__h*/
