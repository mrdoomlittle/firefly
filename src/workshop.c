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
# include "gui/btn.h"
# include "graphics/colour.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "graphics/fill.h"
# include "system/io.h"
# include "system/sched.h"
# include "event.h"
struct {
	
} workshop;
ffly_gui_btnp btn;
ff_i16_t pt_x = 0, pt_y = 0;
ff_i8_t pt_state;
void front() {
	ffly_gui_btn_draw(btn, ffly_frame(__frame_buff__), WIDTH, HEIGHT);
}

static void(*draw)(void) = front;

# include "types/wd_event_t.h"
void ffly_workshop_start() {
	ff_u64_t cc = 0; //cycle count
	while(1) {
		draw();
		ffly_nanosleep(0, 1000000);
		ffly_grp_unload(&__ffly_grp__);
		if (!ff_duct_serve())
			break;
		ff_eventp event;
		while(!ff_event_poll(&event)) {
			if (event->kind == _ffly_wd_ek_btn_press || event->kind == _ffly_wd_ek_btn_release) {
				pt_x = ((ffly_wd_event_t*)event->data)->x;
				pt_y = ((ffly_wd_event_t*)event->data)->y;
				if (event->kind == _ffly_wd_ek_btn_press) {
					pt_state = 0;
				} else if (event->kind == _ffly_wd_ek_btn_release) {
					pt_state = -1;
				}
			}
			
			ffly_printf("event.\n");
			ff_event_del(event);
		}
		ffly_sched_clock_tick(1);
		ffly_scheduler_tick();
//		ffly_printf("%u\n", cc++);
	}
}

void static bt_press(ffly_gui_btnp __btn, void *__arg) {
	ffly_printf("button press.\n");
	ffly_pixfill(btn->texture, 20*20, ffly_colour(66, 194, 224, 255));
}

void static bt_release(ffly_gui_btnp __btn, void *__arg) {
	ffly_printf("button, release.\n");
	ffly_pixfill(btn->texture, 20*20, ffly_colour(2, 52, 132, 255));
}

void static bt_hover(ffly_gui_btnp __btn, void *__arg) {
	ffly_printf("button hover.\n");
}

void ffly_workshop_init() {
	ffly_grp_prepare(&__ffly_grp__, 20);
	ff_set_frame_size(WIDTH, HEIGHT);
	ff_graphics_init();

	ff_duct_open(FF_PIPE_CREAT);
	ffly_mem_set(ffly_frame(__frame_buff__), 0, (WIDTH*HEIGHT)*4);
	ff_duct_listen();

	ffly_queue_init(&ffly_event_queue, sizeof(ff_eventp));

	ffly_scheduler_init();
	ff_u8_t *tex = (ff_u8_t*)__ffly_mem_alloc(20*20*4);
	ffly_pixfill(tex, 20*20, ffly_colour(2, 52, 132, 255));
	btn = ffly_gui_btn_creat(tex, 20, 20, 20, 20);
	btn->pt_x = &pt_x;
	btn->pt_y = &pt_y;
	btn->press = bt_press;
	btn->hover = bt_hover;
	btn->release = bt_release;
	btn->pt_state = &pt_state;
}

void ffly_workshop_de_init() {
	ffly_queue_de_init(&ffly_event_queue);
	ff_duct_close();
	ff_graphics_de_init();
	ffly_plate_cleanup();
	__ffly_mem_free(btn->texture);
	ffly_gui_btn_destroy(btn);
	ffly_scheduler_de_init();
	ff_event_cleanup();
	ffly_grp_cleanup(&__ffly_grp__);
	ffly_grj_cleanup();
}

ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_workshop_init();
	ffly_workshop_start();
	ffly_workshop_de_init();
}
