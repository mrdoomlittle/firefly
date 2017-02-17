# include "src/engine.hpp"
# include "src/graphics/draw_rect.hpp"
void game_loop(boost::int8_t __info, mdl::ffly_client::portal_t *__portal) {
//	if (!__portal-> server_connected())
//		__portal-> connect_to_server("192.168.0.100", 0);

//	if (!__portal-> server_connected()) return;

	mdl::firefly::graphics::colour_t rect_colour;

	rect_colour.r = 255;
	rect_colour.g = 0;
	rect_colour.b = 0;
	rect_colour.a = 255;

	printf("%d\n", __portal-> fps_count());

	mdl::firefly::graphics::draw_rect(
		__portal-> pixbuff, 24, 24, 100, 100, rect_colour, 256, 256
	);
}

int main(int argc, char const *argv[]) {
	mdl::ffly_client client(256, 256);
	client.begin("Example Game", game_loop);

	return 0;
}
