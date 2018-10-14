# include "workshop.h"
# include "graphics/pipe.h"
# include "memory/plate.h"
# include "system/nanosleep.h"
# include "graphics/frame_buff.h"
# include "duct.h"
# include "system/pipe.h"
# include "dep/mem_set.h"
# include "graphics.h"
# include "layer.h"
# include "graphics/colour.h"
# define __ffly_debug
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "graphics/fill.h"
# include "system/io.h"
# include "system/sched.h"
# include "event.h"
# include "carriage.h"
# include "graphics.h"
# include "workshop/font_forge.h"
struct ff_workshop workshop;

ff_i16_t static pt_x = 0, pt_y = 0;
ff_i8_t static pt_state;
enum {
	_bt_fontforge
};
/*
	TODO:
		add ff_i8_t for return is no such 'what?' is possible
	obtain crap that the user controls like mouse pointer coords,state, etc
*/
void static
_get(ff_u8_t __what, long long __d, void *__arg) {
	switch(__what) {
		case 0x00:
			*(ff_i16_t**)__d = &pt_x;
		break;
		case 0x01:
			*(ff_i16_t**)__d = &pt_y;
		break;
		case 0x02:
			*(ff_i8_t**)__d = &pt_state;
		break;
	}
}

void front(void) {
	ffly_gui_window_draw(&workshop.window);
	ffly_gui_btn_draw(workshop.fontforge);
}

void(*tick)(void) = front;
ff_u16_t sf;
ff_dcp static dc;
# include "m.h"
# include "types/wd_event_t.h"
void ffly_workshop_start() {
	sf = BRON_CONTEXT->stack;
	ff_u64_t cc = 0; //cycle count
	while(1) {
		bron_start();
		ffly_pixfill(WIDTH*HEIGHT, ffly_colour(255, 255, 255, 255), 0);
		tick();
		ff_eventp event;
		while(!ff_event_poll(&event)) {
			if (event->kind == _ffly_wd_ek_btn_press || event->kind == _ffly_wd_ek_btn_release) {
				pt_x = ((ffly_wd_event_t*)event->data)->x;
				pt_y = ((ffly_wd_event_t*)event->data)->y;
				ffly_carriage_put(_ff_carr0);
				ffly_carriage_wait(_ff_carr0);
				ffly_carriage_reset(_ff_carr0);
				if (event->kind == _ffly_wd_ek_btn_press) {
					pt_state = 0;
				} else if (event->kind == _ffly_wd_ek_btn_release) {
					pt_state = -1;
				}
			}
			
			ffly_printf("event.\n");
			ff_event_del(event);
		}

		// 30 rps
		ffly_nanosleep(0, 30000000);
		ffly_grp_unload(&__ffly_grp__);

		ffly_fb_copy(__frame_buff__);
		bron_finish();
		bron_done();
		BRON_CONTEXT->stack = sf;

		ffly_fb_yank(__frame_buff__);
		if (!ff_duct_serve(dc))
			break;
		ffly_printf("\e[1Jcycle count: %u\n", cc++);
		struct ffly_meminfo mi;
		ffly_meminfo(&mi);
		ffly_dmi(&mi);
		ffly_fdrain(ffly_out);
	}
}

void static
bt_press(ffly_gui_btnp __btn, void *__arg) {
	ffly_printf("button press.\n");
	if (__btn->id == _bt_fontforge) {
		ffly_printf("fontforge.\n");
		ws_fontforge_init();
		tick = ws_fontforge_tick;
	}
}

void static
bt_release(ffly_gui_btnp __btn, void *__arg) {
	ffly_printf("button, release.\n");
}

void static bt_hover(ffly_gui_btnp __btn, void *__arg) {
	ffly_printf("button hover.\n");
}

ff_u8_t *tex0, *tex1;
# include "ui/text.h"
# include "font.h"
ff_u16_t static fb;
void ffly_workshop_init() {
	workshop.cu = workshop.cubuf;
	ffly_bron_driver(_bron_dd_nought, &BRON_CONTEXT->driver);
	BRON_CONTEXT->stack = 0;
    bron_setctx(bron_ctx_new());
    fb = bron_fb_new(WIDTH, HEIGHT);
    bron_fb_set(fb);
	bron_done();
	ffly_grp_prepare(&__ffly_grp__, 200);
	ff_set_frame_size(WIDTH, HEIGHT);
	ff_graphics_init();

	dc = ff_duct_open(FF_PIPE_CREAT);
	ff_duct_listen(dc);

	ffly_queue_init(&ffly_event_queue, sizeof(ff_eventp));

	ffly_scheduler_init(0);

	tex0 = (ff_u8_t*)__ffly_mem_alloc(76*76*4);
	ffly_mem_set(tex0, 0, 76*76*4);

	ffly_gui_btnp btn;
	btn = ffly_gui_btn_creat(tex0, 76, 76, 0, 0, _get, NULL);
	btn->press = bt_press;
	btn->hover = bt_hover;
	btn->release = bt_release;
	btn->id = _bt_fontforge;
	workshop.fontforge = btn;
	ffly_gui_btn_sched(btn);
	ffly_gui_btn_enable(btn);
	
	ffly_pallet_write(&btn->texture, tex0, 76, 76, 0, 0);

	ffly_gui_window_init(&workshop.window, 64, 64, 128, 128);

	ff_u8_t static pixels[64*64*4];
	ffly_mem_set(pixels, 144, 64*64*4);

	ffly_fontp font;
	ffly_ui_textp text;
	text = ffly_ui_text_creat("012");
	font = ffly_font_new();
	ffly_font_init(font, _font_driver_typo);

	ffly_font_face(font, "test.mcd");
	font->driver.scale(0);
	ffly_ui_text_font(text, font);

	ffly_ui_text_draw(text, pixels, 0, 0, 64, 64);

	ffly_font_destroy(font);
	ffly_ui_text_destroy(text);

	ffly_gui_window_write(&workshop.window, pixels, 64, 64, 0, 0);

}

void ffly_workshop_de_init() {
	void(**cu)(void);
	cu = workshop.cubuf;
	while(cu != workshop.cu) {
		(*(cu++))();
	}
	bron_fb_destroy(fb);
	bron_done();
//	ffly_tiles_usched();
	ffly_queue_de_init(&ffly_event_queue);
	ff_duct_close(dc);
	ff_graphics_de_init();
	ffly_plate_cleanup();
	__ffly_mem_free(tex0);
	ffly_gui_window_de_init(&workshop.window);
	ffly_gui_btn_destroy(workshop.fontforge);
	ff_event_cleanup();
	ffly_grp_cleanup(&__ffly_grp__);
	ffly_grj_cleanup();
	ffly_tile_cleanup();
	ffly_scheduler_de_init();
	ffly_carriage_cleanup();
}

ff_uint_t ws_width;
ff_uint_t ws_height;
# include "clay.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	struct ffly_clay clay;
	ffly_clay_init(&clay);
	ffly_clay_load(&clay, "../clay/workshop.clay");
	ffly_clay_read(&clay);

	void *screen;
	screen = ffly_clay_get("screen", &clay);

	ws_width = clay_16(ffly_clay_tget("width", screen));
	ws_height = clay_16(ffly_clay_tget("height", screen));
	ffly_printf("screen, width: %u, height: %u\n", ws_width, ws_height);
	ffly_clay_de_init(&clay);

	ffly_workshop_init();
	ffly_workshop_start();
	ffly_workshop_de_init();
}
