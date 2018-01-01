# include "btn.h"
# include "../ffly_def.h"
# include "../system/event.h"
# include "../system/errno.h"
# include "../system/event_kind.h"
# include "../system/io.h"
# include "../types/wd_event_t.h"
# include "../graphics/draw.h"
// should use event .xa .ya from window
// will be called every time new event is pushed
ffly_err_t static eir(ffly_event_t *__event, void *__arg_p) {
    if (__event->kind == _ffly_wd_ek_btn_press)
        ffly_gui_btn_handle((struct ffly_gui_btn*)__arg_p, __event);
}

ffly_err_t ffly_gui_btn_draw(ffly_gui_btnp __btn, ffly_pixelmap_t __pixelbuff, mdl_u16_t __width, mdl_u16_t __height) {
    ffly_err_t err;
    if (_err(err = ffly_pixeldraw(__btn->xa, __btn->ya, __pixelbuff, __width, __btn->texture, __btn->width, __btn->height))) {
        return err;
    }
    return FFLY_SUCCESS;
}

ffly_err_t ffly_gui_btn_handle(ffly_gui_btnp __btn, ffly_event_t *__event) {
    if (!__btn->enabled) return FFLY_SUCCESS;
    //reset
    __btn->hovering = ffly_false;
    __btn->pressed = ffly_false;
    mdl_i16_t pt_xa, pt_ya;

    if (!__btn->pt_xa)
        pt_xa = ((ffly_wd_event_t*)__event->data)->x;
    else
        pt_xa = *__btn->pt_xa;

    if (!__btn->pt_ya)
        pt_ya = ((ffly_wd_event_t*)__event->data)->y;
    else
        pt_ya = *__btn->pt_ya;

    if (pt_xa >= __btn->xa && pt_ya >= __btn->ya) {
        if (pt_xa < __btn->xa+__btn->width && pt_ya < __btn->ya+__btn->height) {
                __btn->hovering = ffly_true;
            if (__event->kind == _ffly_wd_ek_btn_press) {
                __btn->pressed = ffly_true;
                ffly_printf("gui button pressed.\n");
            }
        }
    }
    return FFLY_SUCCESS;
}

ffly_err_t ffly_gui_btn_enable_ir(ffly_gui_btnp __btn) {
    ffly_add_eir(&eir, __btn);
    ffly_fprintf(ffly_out, "added ir %p\n", __btn);
}

ffly_bool_t ffly_gui_btn_pressed(ffly_gui_btnp __btn) {
    return __btn->pressed;
}

ffly_bool_t ffly_gui_btn_hovering(ffly_gui_btnp __btn) {
    return __btn->hovering;
}

