# ifndef __ffly__env__h
# define __ffly__env__h
# include "ffint.h"
void envinit(void);
void envcleanup(void);
void envload(char const**);
ff_i8_t envset(char const*, char const*);
char const *envget(char const*);
char const **getenv(void);
# endif /*__ffly__env__h*/
