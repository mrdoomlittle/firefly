#define __uibtn_internal
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
#define BT_AREA		0x01
#define BT_POS		0x02
# include "../graphics/chamber.h"
# include "../oddity.h"
#define isbit(__bits, __bit)\
	(((__bits)&(__bit)) == (__bit))
void ffly_ui_btn_fnode(ffly_ui_btnp __btn, ff_i16_t __x, ff_i16_t __y) {
	struct f_node *n;
	n = (struct f_node*)__ffly_mem_alloc(sizeof(struct f_node));
	n->x = __x;
	n->y = __y;
	
	n->next = __btn->node;
	__btn->node = n;
}

void static draw(long long __arg) {
	ffly_ui_btnp b;
	b = (ffly_ui_btnp)__arg;
	ffly_fprintf(ffly_log, "button draw.\n");
	ffly_pallet_draw(&b->texture, b->x, b->y);
}

#define setarea(__bt, __width, __height)\
	__bt->bits |= BT_AREA;\
	__bt->width = __width;\
	__bt->height = __height;
#define setpos(__bt, __x, __y)\
	__bt->bits |= BT_POS;\
	__bt->x = __x;\
	__bt->y = __y;

void ffly_ui_btn_set(long long __struc, ffly_ui_btnp __bt, ff_u8_t __flags) {
	struct ffly_ui_btn_struc *srt;
	srt = (struct ffly_ui_btn_struc*)__struc;
	if (isbit(__flags, FF_UI_BS_AREA)) {
		setarea(__bt, srt->width, srt->height);
	}
	if (isbit(__flags, FF_UI_BS_POS)) {
		setpos(__bt, srt->x, srt->y)
	}
}

void ffly_ui_btn_setarea(ffly_ui_btnp __btn, ff_u16_t __width, ff_u16_t __height) {
	setarea(__btn, __width, __height);
}

void ffly_ui_btn_setpos(ffly_ui_btnp __btn, ff_u16_t __x, ff_u16_t __y) {
	setpos(__btn, __x, __y);
}

/*
	*rename ????
	i dont know what to name it 

*/
void ffly_ui_btn_compose(ffly_ui_btnp __btn) {
	if (!isbit(__btn->bits, BT_POS|BT_AREA)) {
		ffly_printf("sorry but the button pos or area has not been set.\n");
		return;
	}

	ffly_pallet_init(&__btn->texture, __btn->width, __btn->height, _ffly_tile_16);
}

void
ffly_ui_btn_init(ffly_ui_btnp __btn, ff_u8_t __flags, void(*__get)(ff_u8_t, long long, long long), long long __gt_arg) {
	__btn->pressed = -1;
	__btn->hovering = -1;
	__btn->node = NULL;
	__btn->x = 0;
	__btn->y = 0;
	__btn->width = 0;
	__btn->height = 0;
	__btn->arg = 0;
	__btn->press = NULL;
	__btn->hover = NULL;
	__btn->flags = __flags;
	__btn->bits = 0x00;
	__btn->release = NULL;
	__btn->c = ffly_carriage_add(_ff_carr0);
	__btn->inform = NULL;
	// ignore me(dont hold up everyone and go ahead like im not there)
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

void ffly_ui_btn_text(ffly_ui_btnp __btn, char const *__text) {

}

void ffly_ui_btn_enable(ffly_ui_btnp __btn) {
	ffly_carriage_udud(_ff_carr0);
	__btn->flags |= FFLY_UI_BT_ENABLED;
}

void ffly_ui_btn_disable(ffly_ui_btnp __btn) {
	__btn->flags ^= FFLY_UI_BT_ENABLED;
	ffly_carriage_dud(_ff_carr0);
}

ff_err_t ffly_ui_btn_draw(ffly_ui_btnp __btn) {
	ff_err_t err;
	draw((long long)__btn);
	return FFLY_SUCCESS;
}

ffly_ui_btnp ffly_ui_btn_creat(ff_u8_t __flags, void(*__get)(ff_u8_t, long long, long long), long long __gt_arg) {
	ffly_ui_btnp btn;

	btn = (ffly_ui_btnp)__ffly_mem_alloc(sizeof(struct ffly_ui_btn));
	ffly_ui_btn_init(btn, __flags, __get, __gt_arg);
	return btn;
}

/*
	add to scheduler
*/
ff_i8_t static handle(long long);

void ffly_ui_btn_sched(ffly_ui_btnp __btn) {
	if (!(__btn->flags&BTN_SCHED)) {
		__btn->sched_id = ffly_schedule(handle, __btn, 1);
		__btn->flags |= BTN_SCHED;
	} else {
		caught_oddity;
		// warning
	}
}

void ffly_ui_btn_destroy(ffly_ui_btnp __btn) {
	if (ffly_is_flag(__btn->flags, BTN_SCHED))
		ffly_sched_rm(__btn->sched_id);

	ffly_pallet_de_init(&__btn->texture);

	if ((__btn->flags&FFLY_UI_BT_HOG)>0) {
		ffly_chamber_rm(__btn->ch, __btn->co);
	}
	__ffly_mem_free(__btn);
}

ff_i8_t handle(long long __arg) {
	ffly_ui_btnp btn = (ffly_ui_btnp)__arg;
	if (!(btn->flags&FFLY_UI_BT_ENABLED))
		return -1;
	ff_i16_t pt_x, pt_y;
	ff_i8_t pt_state;
	
	if (ffly_carriage_turn(_ff_carr0, btn->c) == -1)
		return -1;

	if (ffly_carriage_ready(_ff_carr0) == -1)
		return -1;
	
	pt_x = *btn->pt_x;
	pt_y = *btn->pt_y;	
	pt_state = *btn->pt_state;

	ffly_carriage_done(_ff_carr0, btn->c);
	/*
		TODO:
		if press or hover ... is detected pass the call 
		to task_pool to keep the sched from clogging up.
		TODO:
			we want to allow for diffrent geometric shapes for buttons
			so we will itr over each face node and determine if its within sight.
	*/

	ffly_fprintf(ffly_log, "pt_x: %u, pt_y: %u, bt_x: %u, bt_y: %u\n", pt_x, pt_y, btn->x, btn->y);
	if (pt_x >= btn->x && pt_y >= btn->y) {
		if (pt_x < btn->x+btn->width && pt_y < btn->y+btn->height) {
			if (!pt_state) {
				if (btn->pressed != 0) {
					btn->pressed = 0;
					if (btn->press != NULL) 
						btn->press(btn, btn->arg);
				}
			} else {
				if (btn->pressed != -1) {
					btn->pressed = -1;
					if (btn->release != NULL)
						btn->release(btn, btn->arg);
				}
			}
			btn->hovering = 0;
			if (btn->hover != NULL)
				btn->hover(btn, btn->arg);
		} else
			goto _out;
	} else
		goto _out;
	return -1;
_out:
	btn->hovering = -1;
	return -1;
}

ff_err_t ffly_ui_btn_enable_ir(ffly_ui_btnp __btn) {
//	ffly_add_eir(&eir, __btn);
	ffly_fprintf(ffly_out, "added ir %p\n", __btn);
}

ff_i8_t ffly_ui_btn_pressed(ffly_ui_btnp __btn) {
	return __btn->pressed;
}

ff_i8_t ffly_ui_btn_hovering(ffly_ui_btnp __btn) {
	return __btn->hovering;
}

