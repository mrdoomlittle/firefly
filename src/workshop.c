#define __noninternal
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
# include "linux/time.h"
# include "graphics/chamber.h"
# include "context.h"
# include "workshop/front.h"

struct ff_workshop workshop;

ff_i16_t static pt_x = 0, pt_y = 0;
ff_i8_t static pt_state;
ff_u8_t static m_bits = 0x00;
/*
	TODO:
		add ff_i8_t for return is no such 'what?' is possible
	obtain crap that the user controls like mouse pointer coords,state, etc
	"POINTER FORM/TO!"

	i could just pass thru init func but its less flexable and i dont have all the time in the world.
*/
void static
_get(ff_u8_t __what, long long __d, long long __arg) {
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
		case 0x03:
			*(ff_u8_t**)__d = &m_bits;
		break;
	}
}

void(*tick)(void) = NULL;
ff_dcp static dc;
# include "m.h"
# include "types/wd_event_t.h"
# include "log.h"
# include "linux/time.h"
ff_i8_t static
workshop_tick(void) {
	struct timespec start, stop;
	ffly_pixfill(WIDTH*HEIGHT, ffly_colour(255, 255, 255, 255), 0);
	tick();
	ffly_chamber_run(0);
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
			m_bits |= 0x01;

			ffly_carriage_put(_ff_carr0);
			ffly_carriage_wait(_ff_carr0);
			ffly_carriage_reset(_ff_carr0);
		}
		
		ffly_printf("event.\n");
		ff_event_del(event);
	}

	ffly_printf("----> %u, %u, %d\n", pt_x, pt_y, pt_state);

	m_bits = 0x00;
	clock_gettime(CLOCK_MONOTONIC, &start);
	if (!ff_duct_serve(dc))
		return -1;
	clock_gettime(CLOCK_MONOTONIC, &stop);
	ff_log("time taken at workshop sec: %u, ns: %u\n", stop.tv_sec-start.tv_sec, stop.tv_nsec-start.tv_nsec);
	return 0;
}

void static
workshop_init(void) {
	__ffly__chamber__ = ffly_chamber_new();

	dc = ff_duct(0, FF_PIPE_CREAT);
	duct(dc, open);
	duct(dc, listen);

	_queue_loadfuncs(&__ctx(event_queue));
	_queue_init(&__ctx(event_queue), sizeof(ff_eventp));
	workshop.get = _get;
	ws_front();
}

void static
workshop_de_init(void) {
	workshop.de_init();
//	ffly_tiles_usched();
	ffly_queue_de_init(&__ctx(event_queue));
	duct(dc, close);
	ff_event_cleanup();
	ffly_carriage_cleanup();
}

ff_uint_t ws_width;
ff_uint_t ws_height;
# include "clay.h"
# include "engine/loop.h"
# include "engine/entry.h"
# include "engine/view.h"
# include "engine/init.h"
ff_i8_t static
_init(void) {
	workshop_init();
}

ff_i8_t static
_de_init(void) {
	workshop_de_init();
}

// setup
void ffe_entry(void) {
	ffe_iflgs ^= FF_E_GR;
	ffe_init = _init;
	ffe_de_init = _de_init;
	ffe_tick = workshop_tick;

	struct ffly_clay clay;
	_clay_init(&clay);
	_clay_load(&clay, "../clay/workshop.clay");
	_clay_read(&clay);

	void *screen;
	screen = _clay_get("screen", &clay);

	ws_width = _clay_16(_clay_tget("width", screen));
	ws_height = _clay_16(_clay_tget("height", screen));
	ffly_printf("screen, width: %u, height: %u\n", ws_width, ws_height);

	_clay_de_init(&clay);

	// set frame size
	setframesize(ws_width, ws_height);
}
