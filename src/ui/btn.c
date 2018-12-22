# include "btn.h"
# include "../ffly_def.h"
# include "../system/event.h"
# include "../system/errno.h"
# include "../system/io.h"
# include "../types/wd_event_t.h"
# include "../graphics/draw.h"
# include "../system/task_pool.h"
# include "../system/mutex.h"
# include "../system/sched.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../system/flags.h"
# include "../carriage.h"
#define BTN_SCHED 0x01
# include "../graphics/chamber.h"
# include "../oddity.h"
void static draw(long long __arg) {
	ffly_gui_btnp b;
	b = (ffly_gui_btnp)__arg;
	ffly_fprintf(ffly_log, "button draw.\n");
	ffly_pallet_draw(&b->texture, b->x, b->y);
}

void
ffly_gui_btn_init(ffly_gui_btnp __btn, ff_u8_t *__texture, ff_u16_t __width, ff_u16_t __height, ff_u8_t __flags,
	ff_u16_t __x, ff_u16_t __y, void(*__get)(ff_u8_t, long long, void*), void *__gt_arg)
{
	ffly_pallet_init(&__btn->texture, __width, __height, _ffly_tile_16);
	__btn->pressed = -1;
	__btn->hovering = -1;
	__btn->x = __x;
	__btn->y = __y;
	__btn->width = __width;
	__btn->height = __height;
	__btn->arg_p = NULL;
	__btn->press = NULL;
	__btn->hover = NULL;
	__btn->flags = __flags;
	__btn->release = NULL;
	__btn->c = ffly_carriage_add(_ff_carr0);
	ffly_carriage_dud(_ff_carr0);	
	__btn->get = __get;
	__btn->gt_arg = __gt_arg;

	if ((__flags&FFLY_UI_BT_HOG)>0) {
		__btn->co = ffly_chamber_add(draw, (long long)__btn, __btn->ch = __ffly__chamber__);
	}

	__get(0x00, (long long)&__btn->pt_x, __gt_arg);
	__get(0x01, (long long)&__btn->pt_y, __gt_arg);
	__get(0x02, (long long)&__btn->pt_state, __gt_arg);
}

void ffly_ui_btn_text(ffly_gui_btnp __btn, char const *__text) {

}

void ffly_gui_btn_enable(ffly_gui_btnp __btn) {
	ffly_carriage_udud(_ff_carr0);
	__btn->flags |= FFLY_UI_BT_ENABLED;
}

void ffly_gui_btn_disable(ffly_gui_btnp __btn) {
	__btn->flags ^= FFLY_UI_BT_ENABLED;
	ffly_carriage_dud(_ff_carr0);
}

ff_err_t ffly_gui_btn_draw(ffly_gui_btnp __btn) {
	ff_err_t err;
	draw((long long)__btn);
	return FFLY_SUCCESS;
}

ffly_gui_btnp ffly_gui_btn_creat(ff_u8_t *__texture, ff_u16_t __width, ff_u16_t __height, ff_u8_t __flags,
	ff_u16_t __x, ff_u16_t __y,  void(*__get)(ff_u8_t, long long, void*), void *__gt_arg)
{
	ffly_gui_btnp btn;

	btn = (ffly_gui_btnp)__ffly_mem_alloc(sizeof(struct ffly_gui_btn));
	ffly_gui_btn_init(btn, __texture, __width, __height, __flags, __x, __y, __get, __gt_arg);
	return btn;
}

void ffly_gui_btn_sched(ffly_gui_btnp __btn) {
	if (!(__btn->flags&BTN_SCHED)) {
		__btn->sched_id = ffly_schedule(ffly_gui_btn_handle, __btn, 2);
		__btn->flags |= BTN_SCHED;
	} else {
		caught_oddity;
		// warning
	}
}

void ffly_gui_btn_destroy(ffly_gui_btnp __btn) {
	if (ffly_is_flag(__btn->flags, BTN_SCHED))
		ffly_sched_rm(__btn->sched_id);

	ffly_pallet_de_init(&__btn->texture);

	if ((__btn->flags&FFLY_UI_BT_HOG)>0) {
		ffly_chamber_rm(__btn->ch, __btn->co);
	}
	__ffly_mem_free(__btn);
}

ff_i8_t ffly_gui_btn_handle(void *__arg_p) {
	ffly_gui_btnp btn = (ffly_gui_btnp)__arg_p;
	if (!(btn->flags&FFLY_UI_BT_ENABLED))
		return -1;
	ff_i16_t pt_x, pt_y;

	if (ffly_carriage_turn(_ff_carr0, btn->c) == -1)
		return -1;

	if (ffly_carriage_ready(_ff_carr0) == -1)
		return -1;
	
	pt_x = *btn->pt_x;
	pt_y = *btn->pt_y;	

	ffly_carriage_done(_ff_carr0, btn->c);
	/*
		TODO:
		if press or hover ... is detected pass the call 
		to task_pool to keep the sched from clogging up.
	*/

	ffly_fprintf(ffly_log, "pt_x: %u, pt_y: %u, bt_x: %u, bt_y: %u\n", pt_x, pt_y, btn->x, btn->y);
	if (pt_x >= btn->x && pt_y >= btn->y) {
		if (pt_x < btn->x+btn->width && pt_y < btn->y+btn->height) {
			if (!*btn->pt_state) {
				if (btn->pressed != 0) {
					btn->pressed = 0;
					if (btn->press != NULL) 
						btn->press(btn, btn->arg_p);
				}
			} else {
				if (btn->pressed != -1) {
					btn->pressed = -1;
					if (btn->release != NULL)
						btn->release(btn, btn->arg_p);
				}
			}
			btn->hovering = 0;
			if (btn->hover != NULL)
				btn->hover(btn, btn->arg_p);
		} else
			goto _out;
	} else
		goto _out;
	return -1;
_out:
	btn->hovering = -1;
	return -1;
}

ff_err_t ffly_gui_btn_enable_ir(ffly_gui_btnp __btn) {
//	ffly_add_eir(&eir, __btn);
	ffly_fprintf(ffly_out, "added ir %p\n", __btn);
}

ff_i8_t ffly_gui_btn_pressed(ffly_gui_btnp __btn) {
	return __btn->pressed;
}

ff_i8_t ffly_gui_btn_hovering(ffly_gui_btnp __btn) {
	return __btn->hovering;
}

