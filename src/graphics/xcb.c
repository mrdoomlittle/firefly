# include "xcb.h"
# include "../macros.h"
# include "../system/errno.h"
ff_err_t ffly_xcb_query_pointer(struct ffly_xcb_ctx *__ctx, ff_i16_t *__root_xa, ff_i16_t *__root_ya, ff_i16_t *__wd_xa, ff_i16_t *__wd_ya) {
    xcb_generic_error_t *err;
    xcb_query_pointer_cookie_t cookie = xcb_query_pointer(__ctx->conn, __ctx->w);
    xcb_query_pointer_reply_t *reply = xcb_query_pointer_reply(__ctx->conn, cookie, &err);
    if (err) {
        return FFLY_FAILURE;
    }

    __ffly_inna(__root_xa, reply->root_x);
    __ffly_inna(__root_ya, reply->root_y);
    __ffly_inna(__wd_xa, reply->win_x);
    __ffly_inna(__wd_ya, reply->win_y);
    return FFLY_SUCCESS;
}
