# include "src/ffly_engine.hpp"
# include "src/graphics/draw_text.hpp"
# include "src/graphics/draw_skelmap.hpp"
# include "src/graphics/skelmap_loader.hpp"
# include "src/types/skelfont_t.hpp"
# include "src/layer_manager.hpp"
# include "src/graphics/fill_pixmap.hpp"
# include <to_string.hpp>
# include <strcmb.hpp>
# include "src/memory/mem_alloc.h"
# include "src/types/colour_t.hpp"
static mdl::firefly::types::skelfont_t *skelfont;
void game_loop(boost::int8_t __info, mdl::ffly_client::portal_t *__portal) {
//	if (!__portal-> server_connected())
//		__portal-> connect_to_server("192.168.0.100", 0, 1);

//	if (!__portal-> server_connected()) return;
	usleep(1000);
	printf("%d FPS\n", __portal->fps_count());
}

# include "src/skelfont.hpp"
int main(int argc, char const *argv[]) {
	mdl::ffly_client * client = new mdl::ffly_client(640, 640);

//	skelfont = mdl::firefly::load_skelfont("../assets/my_skelfont", &client.asset_manager);
	client->layer.add_layer(640, 640, 0, 0);
	client->layer.add_layer(256, 256, 0, 0);
	client->layer.add_layer(256, 256, 128, 128);
	mdl::firefly::graphics::colour_t colour = {38, 60, 94, 255};
	mdl::firefly::graphics::fill_pixmap(client-> layer.get_layer_pixmap(0), 640, 640, colour);
	colour.r = 0;
	colour.b = 0;
	mdl::firefly::graphics::fill_pixmap(client-> layer.get_layer_pixmap(1), 256, 256, colour);
	colour.r = 244;
	mdl::firefly::graphics::fill_pixmap(client-> layer.get_layer_pixmap(2), 256, 256, colour);

	client->layer.pull_backwards(2);

	mdl::firefly::types::init_opt_t init_options  = {
//	init_options.cam_xlen = 256,
//	init_options.cam_ylen = 256;
		.cam_xlen = 256,
		.cam_ylen = 256
	};

	client->init(init_options);
	client->begin("Example Game", game_loop);

	delete client;
# ifdef DEBUG_ENABLED
	if (mdl::firefly::memory::alloc_amount != mdl::firefly::memory::free_amount)
		fprintf(stderr, "theres a memory leak, allocc: %d, freec: %d\n", mdl::firefly::memory::alloc_count, mdl::firefly::memory::free_count);
# endif

	return 0;
}
