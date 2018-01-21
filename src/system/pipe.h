# ifndef __ffly__pipe__h
# define __ffly__pipe__h
# include <mdlint.h>
# define FF_PIPE_CREAT 0x1
# define FF_PIPE_SHMM 0x2
mdl_uint_t ffly_pipe_get_shmid(mdl_uint_t);
mdl_uint_t ffly_pipe(mdl_uint_t, mdl_u8_t, mdl_uint_t);
void ffly_pipe_write(void*, mdl_uint_t, mdl_uint_t);
void ffly_pipe_read(void*, mdl_uint_t, mdl_uint_t);
void ffly_pipe_close(mdl_uint_t);
void ffly_pipe_listen(mdl_uint_t);
void ffly_pipe_connect(mdl_uint_t);
# endif /*__ffly__pipe__h*/
