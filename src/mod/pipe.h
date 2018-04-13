# ifndef __ffly__mod__pipe__h
# define __ffly__mod__pipe__h
# include "../ffint.h"
ff_uint_t ffmod_pipe_shmid(void);
void ffmod_pipe(void);
void ffmod_pipe_open(ff_uint_t);
void ffmod_pipe_close(void);
ff_uint_t ffmod_pipeno(void);
# endif /*__ffly__mod__pipe__h*/
