# include "ffly_studio.hpp"
bool mdl::ffly_studio::to_shutdown = false;
# include "graphics/draw_grid.hpp"

void __btn_press(mdl::uint_t __btn_id, int __mbtn_id, void *__voidptr, mdl::uint_t *__room_id) {
	mdl::ffly_studio *_this = __voidptr == nullptr? nullptr : (mdl::ffly_studio*)__voidptr;
	if (__room_id == _this->bse_room_id) {
		switch(__btn_id) {
			case 0:
				mdl::ffly_studio::to_shutdown = true;
			break;
			case 1:
				_this->chnage_room(_this->skelc_room_id);
			break;
		}
	} else if (__room_id == _this->skelc_room_id) {
		switch(__btn_id) {
			case 0:
				_this->chnage_room(_this->bse_room_id);
			break;
		}
	}
}

mdl::firefly::types::err_t mdl::ffly_studio::init(firefly::types::init_opt_t __init_options) {
	firefly::init();

	this->_room_manager.init(&this->window);
	this->_room_manager.use_glob_pb_size = true;
	this->_room_manager.set_glob_pb_xlen(this->wd_xaxis_len);
	this->_room_manager.set_glob_pb_ylen(this->wd_yaxis_len);

	if (this->_room_manager.add_room(this->bse_room_id, true) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_studio, failed to add room.\n");
		return FFLY_FAILURE;
	}

	if (this->_room_manager.add_room(this->skelc_room_id, true) != FFLY_SUCCESS) {
		firefly::system::io::printf(stderr, "ffly_studio, failed to add room.\n");
		return FFLY_FAILURE;
	}

	if (this->_room_manager.change_room(this->bse_room_id) == FFLY_FAILURE) {
		firefly::system::io::printf(stderr, "ffly_studio, failed to change room.\n");
		return FFLY_FAILURE;
	}

	firefly::gui::btn btn;
	firefly::types::__id_t btn_id;
	char const *img_files[3] = {
		"../assets/btn_exit",
		"../assets/btn_skel_creator",
		"../assets/btn_main_menu"
	};

	btn.load_btn_ast(this->asset_manager.load_asset(img_files[0], firefly::asset_manager::AST_PNG_FILE), &this->asset_manager);
	btn.create_btn(btn_id, firefly::room_manager::get_btn_manager(this->bse_room_id, &this->_room_manager), firefly::types::__coords__<uint_t>(0, 1));

	this->bse_room.exit_btn = this->_room_manager.get_btn(this->bse_room_id, btn_id);
	this->bse_room.exit_btn->voidptr = this;
	this->bse_room.exit_btn->press_fptr = this->_room_manager.btn_press(&__btn_press);
	this->bse_room.exit_btn->hover_enabled = true;
	this->bse_room.exit_btn->press_enabled = true;
	this->bse_room.exit_btn->enabled = true;

	btn.center_btn(this->bse_room.exit_btn, this->wd_xaxis_len, 0);

	btn.load_btn_ast(this->asset_manager.load_asset(img_files[1], firefly::asset_manager::AST_PNG_FILE), &this->asset_manager);
	btn.create_btn(btn_id, firefly::room_manager::get_btn_manager(this->bse_room_id, &this->_room_manager), firefly::types::__coords__<uint_t>(0, 1 + btn.get_pm_size().yaxis_len + 1));

	this->bse_room.skelc_btn = this->_room_manager.get_btn(this->bse_room_id, btn_id);
	this->bse_room.skelc_btn->voidptr = this;
	this->bse_room.skelc_btn->press_fptr = this->_room_manager.btn_press(&__btn_press);
	this->bse_room.skelc_btn->hover_enabled = true;
	this->bse_room.skelc_btn->press_enabled = true;
	this->bse_room.skelc_btn->enabled = true;

	btn.center_btn(this->bse_room.skelc_btn, this->wd_xaxis_len, 0);

	btn.load_btn_ast(this->asset_manager.load_asset(img_files[2], firefly::asset_manager::AST_PNG_FILE), &this->asset_manager);
	btn.create_btn(btn_id, firefly::room_manager::get_btn_manager(this->skelc_room_id, &this->_room_manager), firefly::types::__coords__<uint_t>(0, 1));

	this->skelc_room.main_menu_btn = this->_room_manager.get_btn(this->skelc_room_id, btn_id);
	this->skelc_room.main_menu_btn->voidptr = this;
	this->skelc_room.main_menu_btn->press_fptr = this->_room_manager.btn_press(&__btn_press);
	this->skelc_room.main_menu_btn->hover_enabled = true;
	this->skelc_room.main_menu_btn->press_enabled = true;
	this->skelc_room.main_menu_btn->enabled = true;

	btn.center_btn(this->skelc_room.main_menu_btn, this->wd_xaxis_len, 0);

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::ffly_studio::begin(char const *__frame_title) {
	if (window.init(this->wd_xaxis_len, this->wd_yaxis_len, __frame_title) != FFLY_SUCCESS)
		return FFLY_FAILURE;
	firefly::init_core_portal(&this->window);

	if (window.begin() != FFLY_SUCCESS)
		return FFLY_FAILURE;

	while(window.wd_handle.is_wd_flag(WD_CLOSED));
	while(window.get_pixbuff() == nullptr);
	_room_manager.set_pixbuff(window.get_pixbuff());

	firefly::types::colour_t bg_colour = {244, 244, 244, 244};
	firefly::gui::window my_window;

	my_window.init(0, 0, 186, 128);
	firefly::types::coords_t<u16_t> mcoords;
	my_window.set_ptrs(&window.button_press(), &window.button_code(), &mcoords);

	firefly::types::pixmap_t tpm;
	firefly::types::_2d_dsize_t<> pms;

	firefly::graphics::load_png_file("../assets/", "test", tpm, pms);

	firefly::data::scale_pixmap(tpm, pms.xaxis_len, pms.yaxis_len, 12, 12, LINEAR_SCALE);
//	firefly::data::scale_pixmap(tpm, pms.xaxis_len, pms.yaxis_len, 12, 12, CUBIC_SCALE);

	firefly::skel_creator skel_creator;
	if (skel_creator.init(this->wd_xaxis_len, this->wd_yaxis_len, 10, 10) != FFLY_SUCCESS) {
		return FFLY_FAILURE;
	}

	do {
		if (window.wd_handle.is_wd_flag(WD_CLOSED)) {
			ffly_studio::to_shutdown = true;
		}

		if (!window.wd_handle.is_wd_flag(WD_WAITING)) continue;
		if (window.wd_handle.is_wd_flag(WD_DONE_DRAW)) continue;
		window.clear_pixbuff(bg_colour);
		mcoords = window.get_mouse_coords().wd;

		if (_room_manager.get_curr_room_id() == this->skelc_room_id) {
			skel_creator.tick(mcoords.xaxis, mcoords.yaxis, window.is_button_press(), window.get_button_code());
			skel_creator.draw(window.get_pixbuff(), this->wd_xaxis_len, this->wd_yaxis_len);
		} else {
			firefly::graphics::draw_pixmap(0, 0, window.get_pixbuff(), this->wd_xaxis_len, this->wd_yaxis_len, tpm, pms.xaxis_len, pms.yaxis_len);

			my_window.draw(window.get_pixbuff(), firefly::types::__dsize__(this->wd_xaxis_len, this->wd_yaxis_len, 1));

			my_window.handle();
		}

		if (this->_room_manager.manage() == FFLY_FAILURE) {
			firefly::system::io::printf(stderr, "ffly_studio: room_manager failed to manage.\n");
			ffly_studio::to_shutdown = true;
		}

		window.wd_handle.add_wd_flag(WD_DONE_DRAW);
		window.wd_handle.rm_wd_flag(WD_WAITING);
	} while(!ffly_studio::to_shutdown);

	skel_creator.de_init();

	printf("shuting down.\n");
	this->_room_manager.de_init();

	cudaDeviceReset();

	window.de_init();
	asset_manager.de_init();

	my_window.de_init();

	firefly::memory::mem_free(tpm);

	printf("done.\n");
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::ffly_studio::de_init() {

}

int main(int argc, char const *argv[]) {
	mdl::ffly_studio ffly_s(640, 640);
	mdl::firefly::types::init_opt_t init_options;
	if (ffly_s.init(init_options) != FFLY_SUCCESS) {
		return 1;
	}

	ffly_s.begin("Firefly Studio");
	ffly_s.de_init();
	return 0;
}
