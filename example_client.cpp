# include "src/engine.hpp"
# include "src/graphics/draw_text.hpp"
# include "src/graphics/draw_skelmap.hpp"
# include "src/graphics/skelmap_loader.hpp"
# include "src/types/skelfont_t.hpp"
# include "src/layer.hpp"
# include "src/graphics/fill_pixmap.hpp"
# include <to_string.hpp>
# include <strcmb.hpp>
# include "src/types/colour_t.hpp"
static mdl::firefly::types::skelfont_t *skelfont;
void game_loop(boost::int8_t __info, mdl::ffly_client::portal_t *__portal) {
	if (!__portal-> server_connected())
		__portal-> connect_to_server("192.168.0.100", 0, 1);

	if (!__portal-> server_connected()) return;
}

# include "src/skelfont.hpp"
int main(int argc, char const *argv[]) {
	mdl::ffly_client client(640, 640);

//	skelfont = mdl::firefly::load_skelfont("../assets/my_skelfont", &client.asset_manager);
	client.layer.add_layer(640, 640, 0, 0);
	client.layer.add_layer(256, 256, 0, 0);
	mdl::firefly::graphics::colour_t colour = {38, 60, 94, 255};
	mdl::firefly::graphics::fill_pixmap(client.layer.get_layer_pixmap(0), 640, 640, colour);

	client.begin("Example Game", game_loop);

	return 0;
}
