# ifndef __ffly__mod__pipe__h
# define __ffly__mod__pipe__h
# include <mdlint.h>
mdl_uint_t ffmod_pipe_shmid(void);
void ffmod_pipe(void);
void ffmod_pipe_open(mdl_uint_t);
void ffmod_pipe_close(void);
mdl_uint_t ffmod_pipeno(void);
# endif /*__ffly__mod__pipe__h*/
