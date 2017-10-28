# include "asset_manager.h"
# include "system/mem_tracker.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
using namespace mdl::firefly;
# include <mdl/str_cmb.h>
# include "system/io.h"
# include "system/file.h"
# include "system/dir.h"
# include "ffly_audio.h"
# include "system/thread.h"
# include "system/task_pool.h"
# include "graphics/window.hpp"
# include <string.h>
# include "layer_manager.hpp"
# include "room_manager.hpp"
# include "types/no_t.h"
extern "C" {
	void ffly_usleep(mdl_u64_t,  mdl_u32_t);
}
int main() {
	room_manager room_m;

	types::no_t *room_no;
	for (;;) {
	room_m.creat_room(room_no, 0);
	room_m.del_room(room_no, 1);
	ffly_usleep(0, 50000000);
	}
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
