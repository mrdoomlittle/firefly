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
# define BTN_SCHED 0x1



void
ffly_gui_btn_init(ffly_gui_btnp __btn, ff_u8_t *__texture, ff_u16_t __width,
	ff_u16_t __height, ff_u16_t __x, ff_u16_t __y)
{
	ffly_pallet_init(&__btn->texture, __width, __height, _ffly_tile_64);
	ffly_pallet_update(&__btn->texture, __texture, __width, __height);
	__btn->pressed = -1;
	__btn->hovering = -1;
	__btn->x = __x;
	__btn->y = __y;
	__btn->width = __width;
	__btn->height = __height;
	__btn->pt_x = NULL;
	__btn->pt_y = NULL;
	__btn->enabled = 0;
	__btn->arg_p = NULL;
	__btn->press = NULL;
	__btn->hover = NULL;
	__btn->flags = 0x0;
}

ff_err_t ffly_gui_btn_draw(ffly_gui_btnp __btn, ffly_palletp __pallet, ff_u16_t __width, ff_u16_t __height) {
	ff_err_t err;
	ffly_fprintf(ffly_log, "button draw.\n");
//	if (_err(err = ffly_pixdraw(__btn->x, __btn->y, __dst, __width, __btn->texture, __btn->width, __btn->height))) {
//		return err;
//	}
	ffly_pallet_copy(&__btn->texture, __pallet, __btn->x, __btn->y);
	return FFLY_SUCCESS;
}

ffly_gui_btnp ffly_gui_btn_creat(ff_u8_t *__texture, ff_u16_t __width,
	ff_u16_t __height, ff_u16_t __x, ff_u16_t __y)
{
	ffly_gui_btnp btn;

	btn = (ffly_gui_btnp)__ffly_mem_alloc(sizeof(struct ffly_gui_btn));
	ffly_gui_btn_init(btn, __texture, __width, __height, __x, __y);
}

void ffly_gui_btn_sched(ffly_gui_btnp __btn) {
	__btn->sched_id = ffly_schedule(ffly_gui_btn_handle, __btn, 2);
	__btn->flags |= BTN_SCHED;
}

void ffly_gui_btn_destroy(ffly_gui_btnp __btn) {
	if (ffly_is_flag(__btn->flags, BTN_SCHED))
		ffly_sched_rm(__btn->sched_id);

	ffly_pallet_de_init(&__btn->texture);
	__ffly_mem_free(__btn);
}

ff_i8_t ffly_gui_btn_handle(void *__arg_p) {
	ffly_gui_btnp btn = (ffly_gui_btnp)__arg_p;
	if (btn->enabled == -1) return FFLY_SUCCESS;
	ff_i16_t pt_x, pt_y;

	pt_x = *btn->pt_x;
	pt_y = *btn->pt_y;	

	/*
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

