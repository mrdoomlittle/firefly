# include "asset_manager.h"
using namespace mdl::firefly;
int main() {
	asset_manager asset_m;

	types::id_t asset_id = ffly_load_asset((void*)&asset_m, "test.wav", _ffly_ak_wav_file);
}
