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
struct {
	
} workshop;
struct ffly_gui_btn btn;

void front() {
	ffly_gui_btn_draw(&btn, ffly_frame(__frame_buff__), WIDTH, HEIGHT);
}

static void(*draw)(void) = front;

void ffly_workshop_start() {
	ff_u64_t cc = 0; //cycle count
	while(1) {
		draw();
		ffly_nanosleep(0, 100000000);
		ffly_grp_unload(&__ffly_grp__);
		if (!ff_duct_serve())
			break;
		ff_eventp event;
		while(!ff_event_poll(&event)) {
			ff_event_del(event);
		}
		ffly_printf("%u\n", cc++);
	}

}

void ffly_workshop_init() {
	ffly_grp_prepare(&__ffly_grp__, 20);
	ff_set_frame_size(WIDTH, HEIGHT);
	ff_graphics_init();

	ff_duct_open(FF_PIPE_CREAT);
	ffly_mem_set(ffly_frame(__frame_buff__), 0, (WIDTH*HEIGHT)*4);
	ff_duct_listen();

	ffly_queue_init(&ffly_event_queue, sizeof(ff_eventp));

	ff_u8_t *tex = (ff_u8_t*)__ffly_mem_alloc(20*20*4);
	ffly_pixfill(tex, 20*20, ffly_colour(2, 52, 132, 255));
	ffly_gui_btn_init(&btn, tex, 20, 20, 20, 20);
}

void ffly_workshop_de_init() {
	ffly_queue_de_init(&ffly_event_queue);
	ff_duct_close();
	ff_graphics_de_init();
	ffly_plate_cleanup();
	__ffly_mem_free(btn.texture);
}

ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_workshop_init();
	ffly_workshop_start();
	ffly_workshop_de_init();
}
