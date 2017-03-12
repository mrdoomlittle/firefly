# include "ffly_studio.hpp"

/*
void test(char __t[][2]) {

}

int main(int argc, char const *argv[]) {
	char tes[][2] = {{0, 0}, {0, 0}};

	test(tes);

}
*/

boost::int8_t mdl::ffly_studio::init(firefly::types::init_opt_t __init_options) {


}

void test(mdl::uint_t __btn_id) {
	printf("btn_hover.\n");
}

boost::int8_t mdl::ffly_studio::begin(char const *__frame_title) {
	FT_Library library;

	if (FT_Init_FreeType(&library)) {
		return FFLY_FAILURE;
	}

	FT_Face face;
	if (FT_New_Face(library, "../TECHNOLIN.ttf", 0, &face)) {
		return FFLY_FAILURE;
	}

	//FT_Set_Char_Size(face, 0, 16 * 64, 300, 300);

	FT_Set_Pixel_Sizes(face, 0, 16);

	char const *text = "FFly Engine";

	firefly::types::bitmap_t bitmap = (firefly::types::bitmap_t)malloc(300 * 300);
	memset(bitmap, 0, 300 * 300);

	std::size_t xoffset = 0;

	for (;*text != '\0'; ++text) {

		if (FT_Load_Char(face, *text, FT_LOAD_RENDER)) {
			return FFLY_FAILURE;
		}

		FT_GlyphSlot g = face-> glyph;

		FT_Bitmap theBitmap = g-> bitmap;

		for (std::size_t y = 0; y != theBitmap.rows; y ++) {
			for (std::size_t x = 0; x != theBitmap.width; x ++) {
				std::size_t pos = (x + xoffset) + (y * 300);
				std::size_t pos2 = x + (y * theBitmap.width);
				bitmap[pos] = theBitmap.buffer[pos2];
				printf("%d	", theBitmap.buffer[pos2]);
			}
			printf("\n");
		}
		xoffset += theBitmap.width + 12;
	}


	firefly::graphics::window window;

	if (window.init(this-> wd_xaxis_len, this-> wd_yaxis_len, __frame_title) != FFLY_SUCCESS)
		return FFLY_FAILURE;

	if (window.begin() != FFLY_SUCCESS)
		return FFLY_FAILURE;

	while(!window.wd_handler.is_wd_flag(WD_OPEN) || window.get_pixbuff() == nullptr) {
	}

	firefly::types::coords_t wd_coords, mouse_coords;

	firefly::gui::btn_manager btn_manager(window.get_pixbuff(), &wd_coords, &mouse_coords, this-> wd_xaxis_len, this-> wd_yaxis_len);

	firefly::types::pixmap_t btn_pixmap = firefly::memory::alloc_pixmap(64, 64, 1);
	memset(btn_pixmap, 200, (64 * 64) * 4);

	firefly::types::coords_t btn_coords = {0, 0};
	btn_manager.create_btn(btn_pixmap, btn_coords, 64, 64);
	btn_manager.set_hover_fptr(0, &test);
	btn_manager.enable_btn(0);
	btn_manager.enable_hover(0);

	firefly::types::skelmap_info_t bp_info;
	bp_info.xaxis_len = 300;
	bp_info.yaxis_len = 300;
	firefly::types::coords_t cc = {66, 66, 0};
	firefly::graphics::colour_t colour = { 49, 57, 71, 255};
	do {
		if (window.wd_handler.is_wd_flag(WD_CLOSED)) {
			break;
		}

		if (!window.wd_handler.is_wd_flag(WD_WAITING)) continue;
		if (window.wd_handler.is_wd_flag(WD_DONE_DRAW)) continue;
		//window.clear_pixbuff();
		firefly::graphics::draw_bitmap(bitmap, 300, 300, window.get_pixbuff(), 640, 640, cc, colour);//, 0,0, bp_info, 640, 640, 1, colour);
		wd_coords = window.get_wd_coords();
		mouse_coords = window.get_mouse_coords();

		//btn_manager.manage();


		window.wd_handler.add_wd_flag(WD_DONE_DRAW);
		window.wd_handler.rm_wd_flag(WD_WAITING);

	} while(1);

	cudaDeviceReset();

	window.wd_handler.add_wd_flag(WD_TO_CLOSE);

	while(!window.wd_handler.is_wd_flag(WD_CLOSED)){}
}

int main(int argc, char const *argv[]) {
	mdl::ffly_studio ffly_s(640, 640);
	ffly_s.begin("Firefly Studio");

}
