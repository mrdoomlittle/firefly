# include "system/io.h"
# include "graphics/image.h"
# include "asset_manager.h"
# include "system/asset.h"
# include "ffly_audio.h"
# include "system/aud_fformat.h"
using namespace mdl::firefly;
int main() {
	ffly_io_init();
    mdl::ffly_audio::init();

    ffly_audio aud;
    ffly_ld_aud(NULL, "atest", _ffly_audf_wav, &aud);
    ffly_aud_write(aud.p, aud.size);

    ffly_aud_drain();
    mdl::ffly_audio::de_init();
    ffly_io_closeup();
}
/*
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
# include "graphics/draw.h"
# include "system/thread.h"

void unloader() {
	while(1) {
		if (!ffly_grp_buff_empty(&__ffly_grp__)) {
//			ffly_printf(ffly_out, "unloading.\n");
			ffly_grp_unload_all(&__ffly_grp__);
		}

		ffly_nanosleep(0, 100000000);
	}
}

struct ttt {
	mdl_u8_t a;
	mdl_u8_t b;
};
# include "asset_manager.h"
# include "data/mem_set.h"
# include "types/err_t.h"
# include "system/errno.h"
# include "room_manager.hpp"
using namespace mdl::firefly::system;
using namespace mdl::firefly;
int main() {
	types::err_t err;
	init();
*/
/*
	asset_manager am;
	types::id_t id = am.load_asset(nullptr, "test.txt", _ffly_ak_raw_file, 0, err);
	io::printf("%s\n", (char*)am.get_asset_data(id));
	am.de_init();
*/
/*
	room_manager room_m;

	types::no_t *a, *b, *c, *d;
	room_m.creat_room(a);
	room_m.creat_room(b);
	room_m.creat_room(c);
	room_m.creat_room(d);

	room_m.del_room(a);
	room_m.del_room(b);
	room_m.del_room(c);
	room_m.del_room(d);
	room_m.creat_room(a);
	room_m.del_room(a);

	de_init();
*/
//	ffly_tid_t tid;
//	ffly_grp_prepare(&__ffly_grp__, 4);

//	system::thread t(&unloader, tid);

//	graphics::window window;
//	window.init(WIDTH, HEIGHT, "Hello World");
//	window.open();
//	window.begin();

//	while(!system::is_flag(window.flags(), FF_FLG_WD_ALIVE));
//	graphics::pixelfill(window.frame_buff(), WIDTH*HEIGHT, graphics::mk_colour(0xFF, 0x0, 0x0, 0xFF));
//	ffly_grp_unload_all(&__ffly_grp__);

//	while(1) {
//		ffly_nanosleep(1, 0);
//		window.display();

		
//	}
/*
	while(1) {
		ffly_nanosleep(1, 0);
		if (system::is_flag(window.flags(), FF_FLG_WD_DEAD)) {
			break;
		}

		window.display();
	}

	system::add_flag(&window.flags(), FF_FLG_WD_OK, 0);
*/

//	ffly_nanosleep(4, 0);
//	window.de_init();
//	window.close();
//	window.cleanup();

//	ffly_nanosleep(0, 10000000);
//	de_init();
//	ffly_grp_cleanup(&__ffly_grp__);

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
//	return 0;
//}
