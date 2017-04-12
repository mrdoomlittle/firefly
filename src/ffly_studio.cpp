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
# include "gui/window.hpp"
# include "data/scale_pixmap.hpp"
bool mdl::ffly_studio::to_shutdown = false;

void __btn_press(mdl::uint_t __btn_id, int __mbtn_id, void *__voidptr, mdl::uint_t *__room_id) {
	mdl::ffly_studio *_this = __voidptr == nullptr? nullptr : (mdl::ffly_studio *)__voidptr;
	if (__room_id == _this-> base_room_id) {
		switch(__btn_id) {
			case 0:
				mdl::ffly_studio::to_shutdown = true;
			break;
			case 1:
				_this-> chnage_room(_this-> skelc_room_id);
			break;
		}
	} else if (__room_id == _this-> skelc_room_id) {
		switch(__btn_id) {
			case 0:
				_this-> chnage_room(_this-> base_room_id);
			break;
		}
	}
}

boost::int8_t mdl::ffly_studio::init(firefly::types::init_opt_t __init_options) {
	this-> _room_manager.init(&window);
	this-> _room_manager.use_glob_pb_size = true;
	this-> _room_manager.set_glob_pb_xlen(this-> wd_xaxis_len);
	this-> _room_manager.set_glob_pb_ylen(this-> wd_yaxis_len);

	if (this-> _room_manager.add_room(this-> base_room_id, true) != FFLY_SUCCESS) {
		fprintf(stderr, "ffly_studio: failed to add room.\n");
		return FFLY_FAILURE;
	}

	if (this-> _room_manager.add_room(this-> skelc_room_id, true) != FFLY_SUCCESS) {
		fprintf(stderr, "ffly_studio: failed to add room.\n");
		return FFLY_FAILURE;
	}

	if (this-> _room_manager.change_room(this-> base_room_id) == FFLY_FAILURE) {
		fprintf(stderr, "ffly_studio: failed to change room.\n");
		return FFLY_FAILURE;
	}

	firefly::gui::btn btn;
	uint_t btn_id;

	btn.load_btn_ast(this-> asset_manager.load_asset("../assets/btn_exit", firefly::asset_manager::AST_PNG_FILE), &this-> asset_manager);
	btn.create_btn(btn_id, &firefly::room_manager::get_btn_manager(this-> base_room_id, &this-> _room_manager), firefly::types::__coords__<uint_t>(0, 1));

	this-> base_room.exit_btn = this-> _room_manager.get_btn(this-> base_room_id, btn_id);
	this-> base_room.exit_btn-> voidptr = this;
	this-> base_room.exit_btn-> press_fptr = this-> _room_manager.btn_press(&__btn_press);
	this-> base_room.exit_btn-> hover_enabled = true;
	this-> base_room.exit_btn-> press_enabled = true;
	this-> base_room.exit_btn-> enabled = true;

	btn.center_btn(this-> base_room.exit_btn, this-> wd_xaxis_len, 0);

	btn.load_btn_ast(this-> asset_manager.load_asset("../assets/btn_skel_creator", firefly::asset_manager::AST_PNG_FILE), &this-> asset_manager);
	btn.create_btn(btn_id, &firefly::room_manager::get_btn_manager(this-> base_room_id, &this-> _room_manager), firefly::types::__coords__<uint_t>(0, 1 + btn.get_pm_size().yaxis_len + 1));

	this-> base_room.skelc_btn = this-> _room_manager.get_btn(this-> base_room_id, btn_id);
	this-> base_room.skelc_btn-> voidptr = this;
	this-> base_room.skelc_btn-> press_fptr = this-> _room_manager.btn_press(&__btn_press);
	this-> base_room.skelc_btn-> hover_enabled = true;
	this-> base_room.skelc_btn-> press_enabled = true;
	this-> base_room.skelc_btn-> enabled = true;

	btn.center_btn(this-> base_room.skelc_btn, this-> wd_xaxis_len, 0);

	btn.load_btn_ast(this-> asset_manager.load_asset("../assets/btn_main_menu", firefly::asset_manager::AST_PNG_FILE), &this-> asset_manager);
	btn.create_btn(btn_id, &firefly::room_manager::get_btn_manager(this-> skelc_room_id, &this-> _room_manager), firefly::types::__coords__<uint_t>(0, 1));

	this-> skelc_room.main_menu_btn = this-> _room_manager.get_btn(this-> skelc_room_id, btn_id);
	this-> skelc_room.main_menu_btn-> voidptr = this;
	this-> skelc_room.main_menu_btn-> press_fptr = this-> _room_manager.btn_press(&__btn_press);
	this-> skelc_room.main_menu_btn-> hover_enabled = true;
	this-> skelc_room.main_menu_btn-> press_enabled = true;
	this-> skelc_room.main_menu_btn-> enabled = true;

	btn.center_btn(this-> skelc_room.main_menu_btn, this-> wd_xaxis_len, 0);

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
	firefly::gui::window my_window;

	my_window.init(0, 0, 186, 128);
	firefly::types::coords_t<boost::uint16_t> mcoords;
	my_window.set_ptrs(&window.button_press(), &window.button_code(), &mcoords);

	firefly::types::pixmap_t tpm;
	firefly::types::_2d_dsize_t<> pms;

	firefly::graphics::load_png_file("../assets/", "test", tpm, pms);

	firefly::data::scale_pixmap(tpm, pms.xaxis_len, pms.yaxis_len, 20, 20, CUBIC_SCALE);

	do {
		if (window.wd_handler.is_wd_flag(WD_CLOSED)) {
			ffly_studio::to_shutdown = true;
		}

		if (!window.wd_handler.is_wd_flag(WD_WAITING)) continue;
		if (window.wd_handler.is_wd_flag(WD_DONE_DRAW)) continue;
		window.clear_pixbuff(bg_colour);
		mcoords = window.get_mouse_coords().wd;


		firefly::graphics::draw_pixmap(0, 0, window.get_pixbuff(), this-> wd_xaxis_len, this-> wd_yaxis_len, tpm, pms.xaxis_len, pms.yaxis_len);
		my_window.draw(window.get_pixbuff(), firefly::types::__dsize__(this-> wd_xaxis_len, this-> wd_yaxis_len, 1));

		my_window.handle();

		if (this-> _room_manager.manage() == FFLY_FAILURE) {
			fprintf(stderr, "ffly_studio: room_manager failed to manage.\n");
			ffly_studio::to_shutdown = true;
		}

		window.wd_handler.add_wd_flag(WD_DONE_DRAW);
		window.wd_handler.rm_wd_flag(WD_WAITING);
	} while(!ffly_studio::to_shutdown);

	printf("shuting down.\n");
	this-> _room_manager.de_init();

	cudaDeviceReset();

	window.de_init();
	asset_manager.de_init();

	my_window.de_init();

	firefly::memory::mem_free(tpm);

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
