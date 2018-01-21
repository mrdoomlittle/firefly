# ifndef __ffly__hatch__h
# define __ffly__hatch__h
enum {
    _ffly_ho_shutdown,
    _ffly_ho_lsvec,
    _ffly_ho_lsmap,
    _ffly_ho_meminfo,
    _ffly_ho_disconnect
};

struct ffly_meminfo {
    mdl_uint_t used;
};
# endif /*__ffly__hatch__h*/
