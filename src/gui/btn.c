# include "btn.h"
# include "../ffly_def.h"
# include "../system/event.h"
# include "../system/errno.h"
# include "../system/event_kind.h"
// will be called every time new event is pushed
ffly_err_t static eir(ffly_event_t *__event, void *__arg_p) {
    if (__event->kind == _ffly_wd_ek_btn_press)
        ffly_ui_btn_handle(__event, (struct ffly_ui_btn*)__arg_p);
}

ffly_err_t ffly_ui_btn_handle(struct ffly_ui_btn *__btn, ffly_event_t *__event) {
    if (!__btn->enabled) return FFLY_SUCCESS;
    mdl_i16_t pt_xa = *__btn->pt_xa, pt_ya = *__btn->pt_ya;
    if (pt_xa >= __btn->xa && pt_ya >= __btn->ya) {
        if (pt_xa < __btn->xa+__btn->width && pt_ya < __btn->ya+__btn->height) {
            __btn->pressed = ffly_true;
        }
    }
    return FFLY_SUCCESS;
}

ffly_err_t ffly_ui_btn_enable_ir(struct ffly_ui_btn *__btn) {
    ffly_add_eir(&eir, __btn);
}

ffly_bool_t ffly_ui_btn_pressed(struct ffly_ui_btn *__btn) {
    return __btn->pressed;
}

ffly_bool_t ffly_ui_btn_hovering(struct ffly_ui_btn *__btn) {
    return __btn->hovering;
}

