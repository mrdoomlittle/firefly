# ifndef __ffly__pipe__h
# define __ffly__pipe__h
# include "../ffint.h"
# include "../types.h"
# define FF_PIPE_CREAT 0x1
# define FF_PIPE_SHMM 0x2
# define FF_PIPE_ENC 0x3
ff_uint_t ffly_pipe_get_shmid(ff_uint_t);
ff_uint_t ffly_pipe(ff_uint_t, ff_u8_t, ff_uint_t, ff_err_t*);
ff_err_t ffly_pipe_write(void*, ff_uint_t, ff_uint_t);
ff_err_t ffly_pipe_read(void*, ff_uint_t, ff_uint_t);
void ffly_pipe_shutoff(ff_uint_t);
void ffly_pipe_close(ff_uint_t);
ff_err_t ffly_pipe_listen(ff_uint_t);
ff_err_t ffly_pipe_connect(ff_uint_t);
# define ffly_pipe_wr8l(__val, __pipe) \
    ffly_pipe_wrl(__val, 1, __pipe)
# define ffly_pipe_wr16l(__val, __pipe) \
    ffly_pipe_wrl(__val, 2, __pipe)
# define ffly_pipe_wr32l(__val, __pipe) \
    ffly_pipe_wrl(__val, 4, __pipe)
# define ffly_pipe_wr64l(__val, __pipe) \
    ffly_pipe_wrl(__val, 8, __pipe)

# define ffly_pipe_rd8l(__pipe, __err) \
    ffly_pipe_rdl(1, __pipe, __err)
# define ffly_pipe_rd16l(__pipe, __err) \
    ffly_pipe_rdl(2, __pipe, __err)
# define ffly_pipe_rd32l(__pipe, __err) \
    ffly_pipe_rdl(4, __pipe, __err)
# define ffly_pipe_rd64l(__pipe, __err) \
    ffly_pipe_rdl(8, __pipe, __err)

ff_err_t ffly_pipe_wrl(ff_u64_t, ff_u8_t, ff_uint_t);
ff_u64_t ffly_pipe_rdl(ff_u8_t, ff_uint_t, ff_err_t*);
# endif /*__ffly__pipe__h*/
