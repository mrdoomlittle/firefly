# include "btn.h"
# include "../ffly_def.h"
# include "../system/event.h"
# include "../system/errno.h"
# include "../system/event_kind.h"
# include "../system/io.h"
# include "../types/wd_event_t.h"
# include "../graphics/draw.h"
# include "../system/task_pool.h"
# include "../system/mutex.h"
// should use event .xa .ya from window
// will be called every time new event is pushed
typedef struct {
    ffly_event_t *event;
    ffly_gui_btnp btn;
} arg;

arg static top[20];
arg static *fresh = top;
arg static *bin[20];
arg static **next = bin;
ff_mlock_t static mutex = FFLY_MUTEX_INIT;

ff_i8_t static *proxy(void *__arg_p) {
    arg *p = (arg*)__arg_p;
    ffly_gui_btn_handle(p->btn, p->event);
    __ffly_mem_free(p->event);
    __ffly_mem_free(p->event->data);
    ffly_mutex_lock(&mutex);
    if (p == fresh-1)
        fresh--;
    else
        *(next++) = p; 
    ffly_mutex_unlock(&mutex); 
    return 0;
}

ff_err_t static eir(ffly_event_t *__event, void *__arg_p) {
    if (__event->kind == _ffly_wd_ek_btn_press) {
        arg *p;
        ffly_mutex_lock(&mutex);
        if (next != bin) 
            p = *(--next);
        else {
            if (fresh >= top+sizeof(top)) {
                return FFLY_FAILURE;
            }
            p = fresh++;
        }
        ffly_mutex_unlock(&mutex);

        p->event = ffly_event_dup(__event);
        p->btn = (ffly_gui_btnp)__arg_p;
        ffly_task_pool_add(&__ffly_task_pool__, &proxy, p); 
    }
    return FFLY_SUCCESS;
}

ff_err_t ffly_gui_btn_draw(ffly_gui_btnp __btn, ffly_pixelmap_t __pixelbuff, ff_u16_t __width, ff_u16_t __height) {
    ff_err_t err;
    if (_err(err = ffly_pixeldraw(__btn->xa, __btn->ya, __pixelbuff, __width, __btn->texture, __btn->width, __btn->height))) {
        return err;
    }
    return FFLY_SUCCESS;
}

ff_err_t ffly_gui_btn_handle(ffly_gui_btnp __btn, ffly_event_t *__event) {
    if (!__btn->enabled) return FFLY_SUCCESS;
    //reset
    __btn->hovering = ffly_false;
    __btn->pressed = ffly_false;
    ff_i16_t pt_xa, pt_ya;

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
                if (__btn->hover_call != NULL)
                    __btn->hover_call(__btn, __btn->arg_p);
            if (__event->kind == _ffly_wd_ek_btn_press) {
                __btn->pressed = ffly_true;
                if (__btn->press_call != NULL)
                    __btn->press_call(__btn, __btn->arg_p);
            }
        }
    }
    return FFLY_SUCCESS;
}

ff_err_t ffly_gui_btn_enable_ir(ffly_gui_btnp __btn) {
    ffly_add_eir(&eir, __btn);
    ffly_fprintf(ffly_out, "added ir %p\n", __btn);
}

ffly_bool_t ffly_gui_btn_pressed(ffly_gui_btnp __btn) {
    return __btn->pressed;
}

ffly_bool_t ffly_gui_btn_hovering(ffly_gui_btnp __btn) {
    return __btn->hovering;
}

