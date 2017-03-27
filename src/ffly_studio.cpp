# include "ffly_studio.hpp"
# include <math.h>
# include <string.h>
# include "graphics/png_loader.hpp"
# include "graphics/colour_blend.hpp"
boost::int8_t mdl::ffly_studio::init(firefly::types::init_opt_t __init_options) {

}

boost::int8_t mdl::ffly_studio::begin(char const *__frame_title) {
	firefly::graphics::window window;

	if (window.init(this-> wd_xaxis_len, this-> wd_yaxis_len, __frame_title) != FFLY_SUCCESS)
		return FFLY_FAILURE;

	if (window.begin() != FFLY_SUCCESS)
		return FFLY_FAILURE;

	while(window.wd_handler.is_wd_flag(WD_CLOSED)) {}

	uint_t *room_id = nullptr;
	this-> _room_manager.add_room(room_id, false);
	firefly::room_manager::room_info_t& room_info = this-> _room_manager.room_info(room_id);
	firefly::room_manager::room_data_t& room_data = this-> _room_manager.room_data(room_id);

	room_data.pixbuff = window.get_pixbuff();

	this-> _room_manager.init(&window);

	do {
		if (window.wd_handler.is_wd_flag(WD_CLOSED)) {
			break;
		}

		if (!window.wd_handler.is_wd_flag(WD_WAITING)) continue;
		if (window.wd_handler.is_wd_flag(WD_DONE_DRAW)) continue;
		window.clear_pixbuff();

		this-> _room_manager.manage(room_id);

		usleep(10000);
		window.wd_handler.add_wd_flag(WD_DONE_DRAW);
		window.wd_handler.rm_wd_flag(WD_WAITING);
	} while(1);

	cudaDeviceReset();

	window.de_init();
}

int main(int argc, char const *argv[]) {
	mdl::ffly_studio ffly_s(640, 640);
	ffly_s.begin("Firefly Studio");

}
