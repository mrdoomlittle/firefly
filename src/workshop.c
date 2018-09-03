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
struct ff_workshop workshop;

ff_i16_t pt_x = 0, pt_y = 0;
ff_i8_t pt_state;
enum {
	_bt_opt,
	_bt_front
};

void opt() {
	ffly_gui_window_draw(&workshop.window, &workshop.frame, WIDTH, HEIGHT);
	ffly_gui_btn_draw(workshop.front, &workshop.frame, WIDTH, HEIGHT);
}

void front() {
	ffly_gui_window_draw(&workshop.window, &workshop.frame, WIDTH, HEIGHT);
	ffly_gui_btn_draw(workshop.opt, &workshop.frame, WIDTH, HEIGHT);
}

static void(*draw)(void) = front;

# include "m.h"
# include "types/wd_event_t.h"
void ffly_workshop_start() {
	ff_u64_t cc = 0; //cycle count
	while(1) {
		ffly_pixfill(ffly_frame(__frame_buff__), WIDTH*HEIGHT, ffly_colour(63, 60, 54, 255));
		draw();
		ffly_pallet_draw(&workshop.frame, ffly_frame(__frame_buff__), WIDTH, HEIGHT, 0, 0);
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

		if (!ff_duct_serve())
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
	if (__btn->id == _bt_opt) {
		//ffly_pixfill(btn->texture, 20*20, ffly_colour(66, 194, 224, 255));
		ffly_printf("switching to opt menu.\n");
		draw = opt;
	} else if (__btn->id == _bt_front) {
		ffly_printf("switching to front menu.\n");
		draw = front;
	}
}

void static
bt_release(ffly_gui_btnp __btn, void *__arg) {
	ffly_printf("button, release.\n");
	if (__btn->id == _bt_opt) {
		//ffly_pixfill(btn->texture, 20*20, ffly_colour(2, 52, 132, 255));
	}
}

void static bt_hover(ffly_gui_btnp __btn, void *__arg) {
	ffly_printf("button hover.\n");
}

ff_u8_t *tex0, *tex1;
void ffly_workshop_init() {
	ffly_pallet_init(&workshop.frame, WIDTH, HEIGHT, _ffly_tile_64);
	ffly_grp_prepare(&__ffly_grp__, 100);
	ff_set_frame_size(WIDTH, HEIGHT);
	ff_graphics_init();

	ff_duct_open(FF_PIPE_CREAT);
	ffly_mem_set(ffly_frame(__frame_buff__), 255, (WIDTH*HEIGHT)*4);
	ff_duct_listen();

	ffly_queue_init(&ffly_event_queue, sizeof(ff_eventp));

	ffly_scheduler_init(0);
	tex0 = (ff_u8_t*)__ffly_mem_alloc(76*76*4);
	tex1 = (ff_u8_t*)__ffly_mem_alloc(76*76*4);
	ffly_pixfill(tex0, 76*76, ffly_colour(255, 52, 132, 255));
	ffly_pixfill(tex1, 76*76, ffly_colour(244, 206, 66, 255));
	ffly_grp_unload(&__ffly_grp__);	

	ffly_gui_btnp btn;
	btn = ffly_gui_btn_creat(tex0, 76, 76, 0, 0);
	btn->pt_x = &pt_x;
	btn->pt_y = &pt_y;
	btn->press = bt_press;
	btn->hover = bt_hover;
	btn->release = bt_release;
	btn->pt_state = &pt_state;
	btn->id = _bt_opt;
	workshop.opt = btn;
	ffly_gui_btn_sched(btn);
	ffly_gui_btn_enable(btn);
	btn = ffly_gui_btn_creat(tex1, 76, 76, 76, 0);
	btn->pt_x = &pt_x;
	btn->pt_y = &pt_y;
	btn->press = bt_press;
	btn->hover = bt_hover;
	btn->release = bt_release;
	btn->pt_state = &pt_state;
	btn->id = _bt_front;
	workshop.front = btn;
	ffly_gui_btn_sched(btn);
	ffly_gui_btn_enable(btn);
	ffly_gui_window_init(&workshop.window, 64, 64, 128, 128);

	ff_u8_t static pixels[12*12*4];
	ffly_mem_set(pixels, 255, 12*12*4);

	ffly_gui_window_write(&workshop.window, pixels, 12, 12, 0, 0);
	ffly_grp_unload(&__ffly_grp__);
	ffly_gui_window_update(&workshop.window);
}

void ffly_workshop_de_init() {
	ffly_tiles_usched();
	ffly_queue_de_init(&ffly_event_queue);
	ff_duct_close();
	ff_graphics_de_init();
	ffly_plate_cleanup();
	__ffly_mem_free(tex0);
	__ffly_mem_free(tex1);
	ffly_gui_window_de_init(&workshop.window);
	ffly_gui_btn_destroy(workshop.opt);
	ffly_gui_btn_destroy(workshop.front);
	ff_event_cleanup();
	ffly_grp_cleanup(&__ffly_grp__);
	ffly_grj_cleanup();
	ffly_pallet_de_init(&workshop.frame);
	ffly_tile_cleanup();
	ffly_scheduler_de_init();
	ffly_carriage_cleanup();
}

ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_workshop_init();
	ffly_workshop_start();
	ffly_workshop_de_init();
}
