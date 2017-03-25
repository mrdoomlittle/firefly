# include "ffly_studio.hpp"
# include <math.h>
# include <string.h>
# include "graphics/png_loader.hpp"
# include "graphics/colour_blend.hpp"
boost::int8_t mdl::ffly_studio::init(firefly::types::init_opt_t __init_options) {

}

static mdl::firefly::pulse_audio pa;

void btn_press(mdl::uint_t __btn_id, int __btn) {
	if (__btn_id == 0) pa.play_sample("firefly", "mrdoomlittle");
	printf("btn pressed.\n");
}

void btn_hover(mdl::uint_t __btn_id) {
	//if (__btn_id == 0) pa.play_sample("firefly", "mrdoomlittle");
	//printf("btn_hover.\n");
}

boost::int8_t mdl::ffly_studio::begin(char const *__frame_title) {
	if (pa.load_file("", "test.wav") != FFLY_SUCCESS) {
		printf("failed to load file.\n");
		return FFLY_FAILURE;
	}
/*
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

	char const *text = "Multiplayer";

	firefly::types::bitmap_t bitmap = (firefly::types::bitmap_t)malloc(288 * 54);
	memset(bitmap, 0, 288 * 54);

	std::size_t xoffset = 0, ss = strlen(text);

	for (;*text != '\0'; ++text) {

		if (FT_Load_Char(face, *text, FT_LOAD_RENDER)) {
			return FFLY_FAILURE;
		}

		FT_GlyphSlot g = face-> glyph;

		FT_Bitmap theBitmap = g-> bitmap;

		for (std::size_t y = 0; y != theBitmap.rows; y ++) {
			for (std::size_t x = 0; x != theBitmap.width; x ++) {
				std::size_t pos = (x + xoffset) + (y * 288);
				std::size_t pos2 = x + (y * theBitmap.width);
				bitmap[pos] = theBitmap.buffer[pos2];
				printf("%d	", theBitmap.buffer[pos2]);
			}
			printf("\n");
		}
		xoffset += theBitmap.width + 6;
	}
	text -= ss;

*/

	uint_t pm_size[2] = {0, 0};
	boost::uint8_t *pm;
	if (firefly::graphics::load_png_file("", "no_texture", pm, pm_size) != FFLY_SUCCESS) return FFLY_FAILURE;

	firefly::font _font;
	_font.init();
	_font.load_font("../", "TECHNOLIN.ttf");
	_font.set_size(16);
	firefly::types::bitmap_t bitmap = _font.make_bitmap("Multiplayer", 6, 288, 54);

	firefly::graphics::window window;

	if (window.init(this-> wd_xaxis_len, this-> wd_yaxis_len, __frame_title) != FFLY_SUCCESS)
		return FFLY_FAILURE;

	if (window.begin() != FFLY_SUCCESS)
		return FFLY_FAILURE;

	while(!window.wd_handler.is_wd_flag(WD_OPEN) || window.get_pixbuff() == nullptr) {
	}


	firefly::types::coords_t<boost::int16_t> wd_coords;
	firefly::types::coords_t<boost::uint16_t> mouse_coords;

	firefly::gui::btn_manager btn_manager(window.get_pixbuff(), &wd_coords, &mouse_coords, this-> wd_xaxis_len, this-> wd_yaxis_len);
	btn_manager.mouse_pressed = &window.wd_handler.button_press;
	btn_manager.mouse_btn_id = &window.wd_handler.button_code;

	firefly::types::pixmap_t btn_pixmap = firefly::memory::alloc_pixmap(288, 54, 1);
	memset(btn_pixmap, 200, (288 * 54) * 4);

	firefly::types::coords_t<> btn_coords = {0, 0};
	btn_manager.create_btn(btn_pixmap, btn_coords, 288, 54);
	btn_manager.set_hover_fptr(0, &btn_hover);
	btn_manager.set_press_fptr(0, &btn_press);
	btn_manager.enable_btn(0);
	btn_manager.enable_hover(0);

	firefly::types::skelmap_info_t bp_info;
	bp_info.xaxis_len = 288;
	bp_info.yaxis_len = 54;
	// (uint_t)((round((288/16)/2) * 16) - round((strlen(text) * (16 + 6)) - 6))
//	firefly::types::coords_t<> cc = { (uint_t)(round(288/2) - round(((strlen(text) * (16 + 6)) - 6 ))/2), (uint_t)(round((54/16)/2) * 16), 0};
	std::size_t yf = 234, xf = 234;
	firefly::system::stop_watch stop_watch;
	stop_watch.begin();
	firefly::types::coords_t<> cc = _font.center();
	firefly::graphics::colour_t colour = { 6, 6, 6, 255};

 auto ant = [&yf, &xf](boost::uint8_t * __pixbuff, uint_t __xaxis, uint_t __yaxis, uint_t rx, uint_t ry, boost::uint8_t *__pm, uint_t xsize, uint_t ysize) -> void {
            std::size_t pos = (__xaxis + (__yaxis * 640)) * 4;
			std::size_t pp = ((rx - xf) + ((ry - yf) * xsize)) * 4;
			boost::uint8_t dr = __pm[pp];//(__pixbuff + pos);
			boost::uint8_t dg = __pm[pp + 1];//(__pixbuff + (pos + 1));
			boost::uint8_t db = __pm[pp + 2];//(__pixbuff + (pos + 2j));
//			if (r == 0) return;	
			
			boost::uint8_t r = dr, g = dg, b = db;
if (pp + 4 < (xsize * ysize) * 4) {
//if (*(__pixbuff + (pos + 4)) != __pm[pp + 4]) {
*(__pixbuff + (pos + 4)) = __pm[pp + 4];
*((__pixbuff + (pos + 4)) + 1) = __pm[(pp + 4) + 1];
*((__pixbuff + (pos + 4)) + 2) = __pm[(pp + 4) + 2];
*((__pixbuff + (pos + 4)) + 3) = __pm[(pp + 4) + 3];
}

if (pp > 4) {
//if (*(__pixbuff + (pos - 4)) != __pm[pp - 4]) {
*(__pixbuff + (pos - 4)) = __pm[pp - 4];
*((__pixbuff + (pos - 4)) + 1) = __pm[(pp - 4) + 1];
*((__pixbuff + (pos - 4)) + 2) = __pm[(pp - 4) + 2];
*((__pixbuff + (pos - 4)) + 3) = __pm[(pp - 4) + 3];
}

if (pp + (xsize * 4) < (xsize * ysize) * 4) {
//if (*(__pixbuff + (pos + (640 * 4))) != __pm[pp + (xsize * 4)]) {
*(__pixbuff + (pos + (640 * 4))) = __pm[pp + (xsize * 4)];
*((__pixbuff + (pos + (640 * 4))) + 1) = __pm[(pp + (xsize * 4)) + 1];
*((__pixbuff + (pos + (640 * 4))) + 2) = __pm[(pp + (xsize * 4)) + 2];
*((__pixbuff + (pos + (640 * 4))) + 3) = __pm[(pp + (xsize * 4)) + 3];
}

if (pp > (xsize * 4)) {
//if (*(__pixbuff + (pos - (640 * 4))) != __pm[pp - (xsize * 4)]) {
*(__pixbuff + (pos - (640 * 4))) = __pm[pp - (xsize * 4)];
*((__pixbuff + (pos - (640 * 4))) + 1) = __pm[(pp - (xsize * 4)) + 1];
*((__pixbuff + (pos - (640 * 4))) + 2) = __pm[(pp - (xsize * 4)) + 2];
*((__pixbuff + (pos - (640 * 4))) + 3) = __pm[(pp - (xsize * 4)) + 3];
}
        };
	std::size_t xsize = 128, ysize = 128;

	boost::uint16_t angle = 130;
	do {
		if (window.wd_handler.is_wd_flag(WD_CLOSED)) {
			break;
		}

		if (!window.wd_handler.is_wd_flag(WD_WAITING)) continue;
		if (window.wd_handler.is_wd_flag(WD_DONE_DRAW)) continue;
		window.clear_pixbuff();
		btn_manager.manage();
		firefly::graphics::draw_bitmap(bitmap, 288, 54, window.get_pixbuff(), 640, 640, cc, colour);//, 0,0, bp_info, 640, 640, 1, colour);
		wd_coords = window.get_wd_coords();
		mouse_coords = window.get_mouse_coords().wd;

		stop_watch.time_point();
		if (stop_watch.get_delta<std::chrono::nanoseconds>() > 100000000) {
			stop_watch.begin();
			if (angle == 360)
				angle = 0;
			else
				angle ++;
		}

		printf("angle: %d\n", angle);
		for (std::size_t y = yf; y != yf + ysize; y ++) {
			for (std::size_t x = xf; x != xf + xsize; x ++) {
				uint_t nx = x - xf, ny = y - yf;

				firefly::maths::rotate_point(nx, ny, angle, xf, yf, -1);

			//	if ((x > xf && x < (xf + xsize)) && (y > yf && y < (yf + ysize)))
			//		ant(window.get_pixbuff(), nx, ny, x, y, pm, xsize, ysize);

				std::size_t pos = (nx + (ny * 640)) * 4;
				std::size_t pp = ((x - xf) + ((y - yf) * xsize)) * 4;

				firefly::graphics::colour_t a = {pm[pp], pm[pp + 1], pm[pp + 2], pm[pp + 3]};
				firefly::graphics::colour_t b = {233, 43, 58, 0};

				firefly::graphics::colour_t f = firefly::graphics::colour_blend(a, b);
				*(window.get_pixbuff() + pos) = f.r;
				*((window.get_pixbuff() + pos) + 1) = f.g;
				*((window.get_pixbuff() + pos) + 2) = f.b;
				*((window.get_pixbuff() + pos) + 3) = f.a;
			}
		}

		window.wd_handler.add_wd_flag(WD_DONE_DRAW);
		window.wd_handler.rm_wd_flag(WD_WAITING);

	} while(1);

	std::free(pm);
	firefly::memory::mem_free(bitmap);

	cudaDeviceReset();

	window.wd_handler.add_wd_flag(WD_TO_CLOSE);

	while(!window.wd_handler.is_wd_flag(WD_CLOSED)){}
}

int main(int argc, char const *argv[]) {
	mdl::ffly_studio ffly_s(640, 640);
	ffly_s.begin("Firefly Studio");

}
