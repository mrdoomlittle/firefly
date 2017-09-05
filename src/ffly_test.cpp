# include "ffly_memory.hpp"
# include "graphics/window.hpp"
# include "graphics/wd_flags.h"
# include "data/mem_set.h"
# include "ffly_audio.h"
# include "asset_manager.hpp"
using namespace mdl::firefly;
# include <stdio.h>
int main() {
	asset_manager am;
	types::id_t a = am.add_asset((mdl_u8_t*)0, 0, 0);
	types::id_t b = am.add_asset((mdl_u8_t*)1, 1, 1);
	types::id_t c = am.add_asset((mdl_u8_t*)2, 2, 2);
	types::id_t d = am.add_asset((mdl_u8_t*)3, 3, 3);

	printf("%u\n", am.get_asset_data(a));
	printf("%u\n", am.get_asset_data(b));
	printf("%u\n", am.get_asset_data(c));
	printf("%u\n", am.get_asset_data(d));

//	printf("main called.\n");
//	ffly_aud_play("", "test", _ffly_audf_wav);
//	while(1);
/*
	graphics::window window;
	window.init(100, 100, "To Tell The World!");

	window.begin();

	while(!ffly_is_flag(window.flags(), FLG_WD_OPEN));
	printf("window open.\n");

	while(!ffly_is_flag(window.flags(), FLG_WD_CLOSED)) {
		if (!ffly_is_flag(window.flags(), FLG_WD_WAITING)) continue;
		if (ffly_is_flag(window.flags(), FLG_WD_DONE_DRAW)) continue;

		ffly_mem_set(window.frame_buff(), 255, 100*100*4);

		ffly_add_flag(&window.flags(), FLG_WD_DONE_DRAW, 0);
		ffly_rm_flag(&window.flags(), FLG_WD_WAITING);
	}

	printf("window closed.\n");
*/
}
