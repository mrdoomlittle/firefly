# ifndef __ffly__pipe__h
# define __ffly__pipe__h
# include <mdlint.h>
# include "../types/err_t.h"
# define FF_PIPE_CREAT 0x1
# define FF_PIPE_SHMM 0x2
# define FF_PIPE_ENC 0x3
mdl_uint_t ffly_pipe_get_shmid(mdl_uint_t);
mdl_uint_t ffly_pipe(mdl_uint_t, mdl_u8_t, mdl_uint_t, ffly_err_t*);
ffly_err_t ffly_pipe_write(void*, mdl_uint_t, mdl_uint_t);
ffly_err_t ffly_pipe_read(void*, mdl_uint_t, mdl_uint_t);
void ffly_pipe_shutoff(mdl_uint_t);
void ffly_pipe_close(mdl_uint_t);
ffly_err_t ffly_pipe_listen(mdl_uint_t);
ffly_err_t ffly_pipe_connect(mdl_uint_t);
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

ffly_err_t ffly_pipe_wrl(mdl_u64_t, mdl_u8_t, mdl_uint_t);
mdl_u64_t ffly_pipe_rdl(mdl_u8_t, mdl_uint_t, ffly_err_t*);
# endif /*__ffly__pipe__h*/
