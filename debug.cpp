# include <firefly/ffly_client.hpp>
# include <firefly/types/colour_t.hpp>
# include <firefly/system/timer.hpp>
# include <firefly/system/io.h>
void engine_loop(mdl::i8_t __info, mdl::ffly_client::portal_t *__portal, void *__arg) {
	usleep(1000);
	static mdl::ffly_client *ffc_ptr = __portal->ffc_ptr;
	mdl::firefly::graphics::fill_pixmap(ffc_ptr->layer.get_layer_pixmap(0), 640, 480, mdl::firefly::graphics::mk_colour(23, 32, 45, 255));
	mdl::firefly::system::io::printf(stdout, "%d FPS\n", __portal->fps_count());
}



mdl::ffly_client _ffly_client(640, 480, (mdl::firefly::types::uni_prop_t){
	mdl::firefly::data::uni_dlen::UNI_DLEN_8,
	mdl::firefly::data::uni_dlen::UNI_DLEN_8,
	mdl::firefly::data::uni_dlen::UNI_DLEN_8
});

int main() {
	mdl::firefly::types::init_opt_t init_options = {
		cam_xlen:256,
		cam_ylen:256
	};

	init_options.add_bse_room = true;
	init_options.change_room = true;
	init_options.add_bse_layer = true;
	init_options.obj_manger_ptr = nullptr;
	init_options.uni_init = false;

	mdl::firefly::types::err_t any_err;
	if ((any_err = _ffly_client.init(init_options)) != FFLY_SUCCESS) {
		fprintf(stderr, "stranded_client: failed to init ffly_client, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}

	if ((any_err = _ffly_client.begin("Stranded Alpha", &engine_loop, NULL)) != FFLY_SUCCESS) {
		fprintf(stderr, "stranded_client: failed to begin, ffly_errno: %d\n", ffly_errno);
		return any_err;
	}
}
