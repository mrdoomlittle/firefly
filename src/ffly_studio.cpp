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
# include "types/id_t.hpp"
bool static *to_shutdown = nullptr;

void btn_press(mdl::uint_t __btn_id, int __b, void *__e) {
	printf("btn has been pressed, id: %d\n", __btn_id);
	if (__btn_id == 0) *to_shutdown = true;

	if (__btn_id == 1) {
		mdl::ffly_studio *_this = (mdl::ffly_studio *)__e;
		_this-> chnage_room(_this-> skelc_room);
	}
}

void _btn_press(mdl::uint_t __btn_id, int __b, void *__e) {
	if (__btn_id == 0) {
		mdl::ffly_studio *_this = (mdl::ffly_studio *)__e;
		_this-> chnage_room(_this-> base_room);
	}
}

boost::int8_t mdl::ffly_studio::init(firefly::types::init_opt_t __init_options) {
	this-> _room_manager.init(&window);
	this-> _room_manager.use_glob_pb_size = true;
	this-> _room_manager.set_glob_pb_xlen(640);
	this-> _room_manager.set_glob_pb_ylen(640);


	if (this-> _room_manager.add_room(this-> base_room, true) != FFLY_SUCCESS) {
		fprintf(stderr, "ffly_studio: failed to add room.\n");
		return FFLY_FAILURE;
	}

	if (this-> _room_manager.add_room(this-> skelc_room, true) != FFLY_SUCCESS) {
		fprintf(stderr, "ffly_studio: failed to add room.\n");
		return FFLY_FAILURE;
	}

	if (this-> _room_manager.change_room(this-> base_room) == FFLY_FAILURE) {
		fprintf(stderr, "ffly_studio: failed to change room.\n");
		return FFLY_FAILURE;
	}

	::to_shutdown = &this-> to_shutdown;

	firefly::gui::btn btn;
	uint_t btn_id;

	if (btn.fload_btn_pm("../assets/", "btn_exit") != FFLY_SUCCESS) {
		return FFLY_FAILURE;
	}

	firefly::gui::btn_manager *btn_manager = &firefly::room_manager::get_btn_manager(this-> base_room, &this-> _room_manager);

	// exit button
	btn.create_btn(btn_id, btn_manager, firefly::types::coords<uint_t>(122, 1));

	firefly::types::btn_t *exit_btn = this-> _room_manager.get_btn(this-> base_room, btn_id);

	exit_btn-> press_fptr = &btn_press;
	exit_btn-> hover_enabled = true;
	exit_btn-> press_enabled = true;

	if (btn.fload_btn_pm("../assets/", "btn_skel_creator") != FFLY_SUCCESS) {
		return FFLY_FAILURE;
	}

	// skelcreator button
	btn.create_btn(btn_id, btn_manager, firefly::types::coords<uint_t>(1, 47));

	firefly::types::btn_t *skelc_btn = this-> _room_manager.get_btn(this-> base_room, btn_id);
	skelc_btn-> voidptr = this;
	skelc_btn-> press_fptr = &btn_press;
	skelc_btn-> hover_enabled = true;
	skelc_btn-> press_enabled = true;

	btn_manager = &firefly::room_manager::get_btn_manager(this-> skelc_room, &this-> _room_manager);

	if (btn.fload_btn_pm("../assets/", "btn_main_menu") != FFLY_SUCCESS) {
		return FFLY_FAILURE;
	}

	// main menu button
	btn.create_btn(btn_id, btn_manager, firefly::types::coords<uint_t>(1, 1));

	firefly::types::btn_t *mmenu_btn = this-> _room_manager.get_btn(this-> skelc_room, btn_id);

	mmenu_btn-> voidptr = this;
	mmenu_btn-> press_fptr = &_btn_press;
	mmenu_btn-> hover_enabled = true;
	mmenu_btn-> press_enabled = true;

	return FFLY_SUCCESS;
}

boost::int8_t mdl::ffly_studio::begin(char const *__frame_title) {
	if (window.init(this-> wd_xaxis_len, this-> wd_yaxis_len, __frame_title) != FFLY_SUCCESS)
		return FFLY_FAILURE;

	if (window.begin() != FFLY_SUCCESS)
		return FFLY_FAILURE;

	while(window.wd_handler.is_wd_flag(WD_CLOSED)) {}

	_room_manager.set_pixbuff(window.get_pixbuff());

	firefly::graphics::colour_t bg_colour = { 244, 244, 244, 244};
	do {
		if (window.wd_handler.is_wd_flag(WD_CLOSED)) {
			this-> to_shutdown = true;
		}

		if (!window.wd_handler.is_wd_flag(WD_WAITING)) continue;
		if (window.wd_handler.is_wd_flag(WD_DONE_DRAW)) continue;
		window.clear_pixbuff(bg_colour);

		if (this-> _room_manager.manage() == FFLY_FAILURE) {
			fprintf(stderr, "ffly_studio: room_manage failed to manage.\n");
			this-> to_shutdown = true;
		}

		usleep(100);
		window.wd_handler.add_wd_flag(WD_DONE_DRAW);
		window.wd_handler.rm_wd_flag(WD_WAITING);
	} while(!this-> to_shutdown);

	printf("shuting down.\n");
	this-> _room_manager.de_init();

	cudaDeviceReset();

	window.de_init();

	printf("done.\n");
	return FFLY_SUCCESS;
}

int main(int argc, char const *argv[]) {
	mdl::ffly_studio ffly_s(640, 640);
	mdl::firefly::types::init_opt_t init_options;
	if (ffly_s.init(init_options) != FFLY_SUCCESS) {
		return 1;
	}

	ffly_s.begin("Firefly Studio");
	printf("Hello\n");
	return 0;
}
