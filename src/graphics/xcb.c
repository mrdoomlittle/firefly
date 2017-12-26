# include "xcb.h"
# include "../macros.h"
# include "../system/errno.h"
ffly_err_t ffly_xcb_query_pointer(struct ffly_xcb_wd *__wd, mdl_i16_t *__root_xa, mdl_i16_t *__root_ya, mdl_i16_t *__wd_xa, mdl_i16_t *__wd_ya) {
    xcb_generic_error_t *err;
    xcb_query_pointer_cookie_t cookie = xcb_query_pointer(__wd->conn, __wd->w);
    xcb_query_pointer_reply_t *reply = xcb_query_pointer_reply(__wd->conn, cookie, &err);
    if (err) {
        return FFLY_FAILURE;
    }

    __ffly_inna(__root_xa, reply->root_x);
    __ffly_inna(__root_ya, reply->root_y);
    __ffly_inna(__wd_xa, reply->win_x);
    __ffly_inna(__wd_ya, reply->win_y);
    return FFLY_SUCCESS;
}
