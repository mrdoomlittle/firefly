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
//	ffly_byte_t *fp;
//	ffly_size_t fsize;
//	if (ffly_ld_aud_file("", "test", _ffly_audf_wav, &fp, &fsize) != FFLY_SUCCESS) {
//		return FFLY_FAILURE;
//	}

//	types::id_t id = am.add_asset((mdl_u8_t*)fp, fsize, 0);
//	*((ffly_aud_fformat_t*)(am.asset_info(id) = __ffly_mem_alloc(sizeof(ffly_aud_fformat_t)))) = _ffly_audf_wav;
/*
	types::id_t b = am.add_asset((mdl_u8_t*)1, 1, 1);
	types::id_t c = am.add_asset((mdl_u8_t*)2, 2, 2);
	types::id_t d = am.add_asset((mdl_u8_t*)3, 3, 3);

	printf("%u\n", am.get_asset_data(a));
	printf("%u\n", am.get_asset_data(b));
	printf("%u\n", am.get_asset_data(c));
	printf("%u\n", am.get_asset_data(d));
*/

	types::id_t id = am.load_asset("test", _ffly_ak_wav_file);

	types::asset_t& ass = am.asset(id);
	ffly_aud_fad_t *aud_fad = (ffly_aud_fad_t*)ass.data;
	ffly_aud_play(aud_fad->fp, aud_fad->fsize, aud_fad->fformat);
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
