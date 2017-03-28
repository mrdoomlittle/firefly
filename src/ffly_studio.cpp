# include "ffly_studio.hpp"
# include <math.h>
# include <string.h>
# include "graphics/png_loader.hpp"
# include "graphics/colour_blend.hpp"
# include "memory/mem_alloc.h"
# include "memory/mem_init.h"
# include "types/pixmap_t.h"
# include "types/colour_t.hpp"
# include "gui/btn_manager.hpp"
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

	uint_t btn_xlen = 64, btn_ylen = 64;

	uint_t btn_id;
	uint_t msize = (btn_xlen * btn_ylen) * sizeof(boost::uint8_t);
	msize *=4;

	firefly::types::pixmap_t btn_pixmap = (firefly::types::pixmap_t)firefly::memory::mem_alloc(msize);
	memset(btn_pixmap, 145, msize);

	this-> _room_manager.create_btn(room_id, btn_id, btn_pixmap, firefly::types::coords<uint_t>(64, 64), 64, 64);

	firefly::gui::btn_manager& btn_manager = firefly::room_manager::get_btn_manager(room_id, &this-> _room_manager);
	btn_manager.set_pb_xlen(640);
	btn_manager.set_pb_ylen(640);

	//firefly::room_manager::get_btn_manager(room_id, &this-> _room_manager).btn_pixmap(btn_id) = btn_pixmap;
	firefly::graphics::colour_t bg_colour = { 244, 244, 244, 244};
	do {
		if (window.wd_handler.is_wd_flag(WD_CLOSED)) {
			break;
		}

		if (!window.wd_handler.is_wd_flag(WD_WAITING)) continue;
		if (window.wd_handler.is_wd_flag(WD_DONE_DRAW)) continue;
		window.clear_pixbuff(bg_colour);

		this-> _room_manager.draw_room(room_id);
		this-> _room_manager.manage(room_id);

		usleep(10000);
		window.wd_handler.add_wd_flag(WD_DONE_DRAW);
		window.wd_handler.rm_wd_flag(WD_WAITING);
	} while(1);

	this-> _room_manager.de_init();

	cudaDeviceReset();

	window.de_init();
}

int main(int argc, char const *argv[]) {
	mdl::ffly_studio ffly_s(640, 640);
	ffly_s.begin("Firefly Studio");

}
