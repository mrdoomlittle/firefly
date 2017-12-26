# include "x11.h"
# include "../macros.h"
# include "../system/errno.h"
ffly_err_t ffly_x11_query_pointer(struct ffly_x11_wd *__wd, mdl_i16_t *__root_xa, mdl_i16_t *__root_ya, mdl_i16_t *__wd_xa, mdl_i16_t *__wd_ya) {
    Window root, child; // not needed for now
    int root_xa, root_ya, wd_xa, wd_ya;
    int unsigned mask;
    XQueryPointer(__wd->d, XRootWindow(__wd->d, 0), &root, &child, &root_xa, &root_ya, &wd_xa, &wd_ya, &mask);    
    __ffly_inna(__root_xa, root_xa);
    __ffly_inna(__root_ya, root_ya);
    __ffly_inna(__wd_xa, wd_xa);
    __ffly_inna(__wd_ya, wd_ya);
    return FFLY_SUCCESS;
}
