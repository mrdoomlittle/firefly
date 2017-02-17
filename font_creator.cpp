# include "src/x11_window.hpp"
# include <eint_t.hpp>
# include <math.h>
# include <signal.h>
# include <emu2d.hpp>
# include <string.h>
# include <fstream>
# include "draw_grid.hpp"
int main(int argc, char const *argv[]) {
	if (argc < 6) {
		printf("usage: x pix count, y pix count, x size, y size, save file.\n");
		return 1;
	}

	mdl::x11_window x11_window;
	mdl::uint_t xpix_amount = atoi(argv[1]);
	mdl::uint_t ypix_amount = atoi(argv[2]);

	mdl::uint_t pix_xlen = atoi(argv[3]);
	mdl::uint_t pix_ylen = atoi(argv[4]);

	mdl::uint_t w_width = xpix_amount * pix_xlen;
	mdl::uint_t w_height = ypix_amount * pix_ylen;

	if (w_width % 2 || w_height  % 2) return 1;


	mdl::uint_t pxlen = (w_width / pix_xlen), pylen = (w_height / pix_ylen);
	boost::uint8_t *__pixmap = static_cast<boost::uint8_t *>(malloc(pxlen * pylen));
	bzero(__pixmap, pxlen * pylen);

	x11_window.begin(w_width, w_height, "Firefly: Font Creator");

	bool already_pressed = false;
	bool pixel_updated = false;

	mdl::uint_t start_x = 0, start_y = 0;

	boost::uint8_t zero_colour[4] = {0, 0, 0, 0};
	boost::uint8_t one_colour[4] = {255, 255, 255, 255};

	mdl::uint_t sizes[2] = {w_width, w_height};
	mdl::uint_t grid_offsets[2] = {pix_xlen , pix_ylen};

/*
	mdl::uint_t spx = 0, spy = 0, epx = 234, epy = 68;

	mdl::uint_t delx = epx - spx;
	mdl::uint_t dely = epx - spx;
	mdl::uint_t delaerr = abs(dely / delx);
	mdl::uint_t error = delaerr - 0.5;
*/
	while(true) {
		if (x11_window.window_closed && pthread_kill(x11_window.window_thread-> native_handle(), 0) != 0) {
			printf("window has been closed.\n");
			break;
		}

		if (!x11_window.waitting) continue;
		if (x11_window.done_drawing) continue;
/*
		std::size_t y = spy;
		for (std::size_t x = spx; x != spx + epx; x ++) {
			mdl::uint_t pix_pos = mdl::emu2d(x, y, w_width, w_height, 4);
			x11_window.pixels[pix_pos] = 255;
			x11_window.pixels[pix_pos + 3] = 255;
			error = error + delaerr;
			if (error > 0.5) {
				y = y + 1;
				error = error - 1.0;
			}
		}


		goto end;
*/
		if (x11_window.button_press) {
			already_pressed = false;
		}

		if (already_pressed) goto skip;

		if (x11_window.button_press && !already_pressed) {
			already_pressed = true;
			pixel_updated = true;
		}

		if (!pixel_updated) goto skip;

		start_x = floor(x11_window.mouse_coords.w_xaxis / pix_xlen);
		start_y = floor(x11_window.mouse_coords.w_yaxis / pix_ylen);
		start_x = start_x * pix_xlen;
		start_y = start_y * pix_ylen;

		//printf("mouseX: %d, mouse Y: %d\n", x11_window.mouse_coords.w_xaxis, x11_window.mouse_coords.w_yaxis);

		for (std::size_t y = start_y; y != start_y + pix_ylen; y ++) {
			for (std::size_t x = start_x; x != start_x + pix_xlen; x ++) {
				mdl::uint_t pix_pos = mdl::emu2d(x, y, w_width, w_height, 4);
				if (x11_window.button_code == 0x1) {
					x11_window.pixels[pix_pos] = one_colour[0];
					x11_window.pixels[pix_pos + 1] = one_colour[1];
					x11_window.pixels[pix_pos + 2] = one_colour[2];
					x11_window.pixels[pix_pos + 3] = one_colour[3];
				} else if (x11_window.button_code == 0x3) {
					x11_window.pixels[pix_pos] = zero_colour[0];
					x11_window.pixels[pix_pos + 1] = zero_colour[1];
					x11_window.pixels[pix_pos + 2] = zero_colour[2];
					x11_window.pixels[pix_pos + 3] = zero_colour[3];
				}

			}
		}

		printf("%dx%d\n", (start_x/pix_xlen), (start_y/pix_ylen));
		if (x11_window.button_code == 0x1) {
			__pixmap[(start_x/pix_xlen) + ((start_y/pix_ylen) * pxlen)] = 0x1;
		} else if (x11_window.button_code == 0x3) {
			__pixmap[(start_x/pix_xlen) + ((start_y/pix_ylen) * pxlen)] = 0x0;
		}

		pixel_updated = true;

		skip:

		draw_grid(x11_window.pixels, sizes, grid_offsets);

		//printf("X: %d, Z: %d.\n", start_x, start_y);

		//usleep(100000);

		x11_window.done_drawing = true;
		x11_window.waitting = false;
	}

	printf("exiting font creator.\n");

	FILE *ofile = fopen(argv[5], "wb");

	fwrite(&pxlen, sizeof(mdl::uint_t), 1, ofile);
	fwrite(&pylen, sizeof(mdl::uint_t), 1, ofile);
	fwrite(__pixmap, 1, (pxlen * pylen), ofile);
	fclose(ofile);

	std::free(__pixmap);

	return 0;
}
