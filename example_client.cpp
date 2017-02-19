# include "src/engine.hpp"
# include "src/graphics/draw_text.hpp"
# include "src/graphics/draw_skelmap.hpp"
# include "src/graphics/skelmap_loader.hpp"
# include "src/types/skelfont_t.hpp"
# include "src/layer.hpp"
static mdl::firefly::types::skelfont_t *skelfont;
void game_loop(boost::int8_t __info, mdl::ffly_client::portal_t *__portal) {
	if (!__portal-> server_connected())
		__portal-> connect_to_server("192.168.0.100", 0, 0);

	if (!__portal-> server_connected()) return;
/*
	mdl::firefly::graphics::colour_t skel_colour;

	skel_colour.r = 50;
	skel_colour.g = 81;
	skel_colour.b = 130;
	skel_colour.a = 255;

	mdl::firefly::graphics::draw_text(
		&__portal-> _this-> asset_manager,
		__portal-> pixbuff,
		skelfont,
		"Hello",
		skel_colour,
		5,
		5,
		2,
		256,
		256,
		2
	);
*/
	printf("%d\n", __portal-> fps_count());


}

# include "src/skelfont.hpp"
int main(int argc, char const *argv[]) {
	mdl::ffly_client client(256, 256);

	skelfont = mdl::firefly::load_skelfont("../assets/my_skelfont", &client.asset_manager);

	mdl::uint_t layer_id = client.layer.add_layer(256, 256, 0, 0);

	mdl::firefly::graphics::colour_t skel_colour;

	skel_colour.r = 50;
	skel_colour.g = 81;
	skel_colour.b = 130;
	skel_colour.a = 160;

	layer_id = client.layer.add_layer(256, 256, 0, 0);

	mdl::firefly::graphics::draw_text(
		&client.asset_manager,
		client.layer.get_layer_pixmap(layer_id),
		skelfont,
		"Example Game",
		skel_colour,
		5,
		5,
		2,
		256,
		256,
		2
	);

	//boost::uint8_t *test = (boost::uint8_t *)malloc((256*256)*4);
	//client.layer.draw_layers(test 256, 256);

	client.begin("Example Game", game_loop);

	return 0;
}
