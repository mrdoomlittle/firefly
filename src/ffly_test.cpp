# include "asset_manager.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
using namespace mdl::firefly;
# include <mdl/str_cmb.h>
# include "system/io.h"
# include "system/file.h"
# include "system/dir.h"
# include "ffly_def.h"
# include "system/nanosleep.h"
# include "firefly.hpp"
# include "graphics/fill.h"
# include "graphics/colour.hpp"
# include "types/off_t.h"
# include "graphics/window.hpp"
# include "graphics/wd_flags.h"
# include "graphics/copy.h"
# define WIDTH 800
# define HEIGHT 800
# include "graphics/pipe.h"
# include "graphics/fill.h"
int main() {
/*
	ffly_io_init();
	ffly_grp_prepare(&__ffly_grp__, 12);

	graphics::fill_pixelmap(NULL, 21299, graphics::mk_colour(0xFF, 0xFF, 0xFF, 0xFF));
	ffly_grp_unload_all(&__ffly_grp__);

	ffly_grp_cleanup(&__ffly_grp__);
	ffly_io_closeup();
*/

	init();
	ffly_grp_prepare(&__ffly_grp__, 12);
	graphics::window window;
	window.init(WIDTH, HEIGHT, "Hello World");
	window.begin();

	mdl_u8_t *buf = (mdl_u8_t*)__ffly_mem_alloc(WIDTH*HEIGHT*4);
	graphics::pixelfill(buf, WIDTH*HEIGHT, graphics::mk_colour(0xFF, 0xFF, 0xFF, 0xFF));
	ffly_grp_unload_all(&__ffly_grp__);

	while(!system::is_flag(window.flags(), FFLY_FLG_WD_ALIVE));

	while(1) {
		ffly_nanosleep(1, 0);
		if (system::is_flag(window.flags(), FFLY_FLG_WD_DEAD)) {
			break;
		}

		graphics::pixelcopy(window.frame_buff(), buf, WIDTH*HEIGHT);
		ffly_grp_unload_all(&__ffly_grp__);
		if (!system::is_flag(window.flags(), FFLY_FLG_WD_DRAW_FRAME))
			system::add_flag(&window.flags(), FFLY_FLG_WD_DRAW_FRAME, 0);

		if (!system::is_flag(window.flags(), FFLY_FLG_WD_NEXT_FRAME)) continue;
		system::rm_flag(&window.flags(), FFLY_FLG_WD_NEXT_FRAME);
	}

	system::add_flag(&window.flags(), FFLY_FLG_WD_OK, 0);

	ffly_nanosleep(0, 10000000);
//	graphics::fill_frame(frame, YA*XA, graphics::mk_colour(0xFF, 0xFF, 0xFF, 0xFF));

	free(buf);
	de_init();
	ffly_grp_cleanup(&__ffly_grp__);

//	ffly_io_closeup();
/*
	room_manager room_m;

	types::no_t *room_no;
	for (;;) {
	room_m.creat_room(room_no, 0);
	room_m.del_room(room_no, 1);
	ffly_usleep(0, 50000000);
	}
*/
//	graphics::window window;
//	window.init(400, 400, "Hello World");

//	window.begin();
//	while(1);
/*
	ffly_mem_track_init(&__ffly_mem_track__);
	ffly_task_pool_init(&__task_pool__, 4);
	usleep(2000000);

	ffly_aud_spec_t aud_spec = {
		.format=_ffly_af_s16_le,
		.rate=48000,
		.chn_c=2
	};

	ffly_audio_init(&aud_spec);
	mdl_u8_t *p = NULL;
	ffly_size_t size;
	ffly_ld_aud_file("a", "test", _ffly_audf_wav, &p, &size);

	printf("%d\n", size-(1<<20));

	ffly_aud_write(p+44, size-44);
	ffly_aud_drain();
	ffly_aud_write(p+44, size-44);
	ffly_aud_drain();

	ffly_audio_de_init();

	__ffly_mem_free(p);
*/
//	printf("size: %u\n", sizeof(mdl_uint_t));
//	char *t = mdl_str_cmb("A", "B", 0);

//	__ffly_mem_free(t);

//	asset_manager asset_m;

//	types::id_t asset_id = ffly_load_asset((void*)&asset_m, NULL, "test", _ffly_ak_wav_file);
//
//	asset_m.de_init();
/*
	ffly_task_pool_de_init(&__task_pool__);
	ffly_thread_cleanup();
	ffly_mem_track_dump(&__ffly_mem_track__);
	ffly_mem_track_de_init(&__ffly_mem_track__);

	printf("finished.\n");
*/
//	printf("mem track: used: %lu bytes - freed: %lu bytes - lost:  bytes\n", ffly_mem_alloc_bc, ffly_mem_free_bc);
	return 0;
}
