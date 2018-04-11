# ifndef __ffly__hatch__h
# define __ffly__hatch__h
# include <mdlint.h>
# include "types/err_t.h"
enum {
    _ffly_ho_shutdown,
    _ffly_ho_lsvec,
    _ffly_ho_lsmap,
    _ffly_ho_meminfo,
    _ffly_ho_login,
    _ffly_ho_logout,
    _ffly_ho_disconnect
};

struct ffly_meminfo {
    mdl_uint_t used;
};

ffly_err_t ffly_hatch_start();
void ffly_hatch_shutoff();
void ffly_hatch_wait();
# endif /*__ffly__hatch__h*/
