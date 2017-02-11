# include "ffly_client.hpp"
# include "png_loader.hpp"
# include "client_gui.hpp"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pulse/simple.h>
#include <pulse/error.h>

# define BUFFSIZE 1024
static const pa_sample_spec sample_spec = {
	.format = PA_SAMPLE_S16LE,
	.rate = 44100,
	.channels = 2
};

mdl::ffly_client::ffly_client(uint_t __window_xlen, uint_t __window_ylen)
	: window_xlen(__window_xlen), window_ylen(__window_ylen) {

}

void test_btn_press(int id) {
	char const *argv[] = {"./client.exec", "test.wav"};

	pa_simple *sample = NULL;
	int ret = 1;
	int any_error = 0;

	int fd;

	if ((fd = open(argv[1], O_RDONLY)) < 0) goto end;

	if (dup2(fd, STDIN_FILENO) < 0) goto end;

	close(fd);

	if (!(sample = pa_simple_new(NULL, argv[0], PA_STREAM_PLAYBACK, NULL, "playback", &sample_spec, NULL, NULL, &any_error))) goto end;

	while (true) {
		uint8_t buff[BUFFSIZE];
		bzero(buff, BUFFSIZE);
 		ssize_t r;

		if ((r = read(STDIN_FILENO, buff, sizeof(buff))) <= 0) {
			if (r == 0) break;
			goto end;
		}

		if (pa_simple_write(sample, buff, (size_t)r, &any_error) < 0) goto end;
	}

	if (pa_simple_drain(sample, &any_error) < 0) goto end;

	end:
	if (sample) pa_simple_free(sample);

	printf("button was pressed.\n");
}

void hover() {
    test_btn_press(0);
}

boost::uint8_t mdl::ffly_client::begin(char const * __frame_title) {
	mdl::x11_window _x11_window;
	_x11_window.begin(this-> window_xlen, this-> window_ylen, __frame_title);

	boost::uint8_t *pixmap = nullptr, *hpixmap = nullptr;
	int unsigned pixmap_size[2];

	if (load_png_file("button.png", pixmap, pixmap_size)) return 1;
	if (load_png_file("hbutton.png", hpixmap, pixmap_size)) return 1;
	if (pixmap == nullptr || hpixmap == nullptr) return 1;

	if (pixmap[0] == hpixmap[0]) return 1;

	printf("pixmap size: %dx%d\n", pixmap_size[0], pixmap_size[1]);

	 while(_x11_window.pixels == nullptr) {}
	mdl::client_gui _client_gui(_x11_window.pixels, &_x11_window.mouse_coords, &_x11_window.button_press, &_x11_window.button_code, this-> window_xlen, this-> window_ylen);

	_client_gui.create_button(pixmap, hpixmap, &hover, &test_btn_press, 64, 64, pixmap_size[0], pixmap_size[1]);
/*
	printf("pixmap size: %dx%d\n", pixmap_size[0], pixmap_size[1]);

	int unsigned a[2] = {(640 * 640)*4, (pixmap_size[0] * pixmap_size[1])*4};
	int unsigned b[2] = {pixmap_size[0], pixmap_size[1]};
*/

	//draw_pixmap(0, 0, x11_window.pixels, pixmap, a, b, 640);
/*
	for (std::size_t o = 0; o != 64; o ++)
		for (std::size_t i = 0; i != 64; i ++)
			x11_window.pixels[(o + (i * 640))*4] = 100;
*/
	do {
		_client_gui.draw_buttons();
		if (_x11_window.button_press) printf("%d\n", _x11_window.button_code);
		usleep(10000);
	} while(1);
}

int main() {
	mdl::ffly_client ffly_client(640, 640);
	ffly_client.begin("Example Game");
}

/*
# include "x11_window.hpp"
# include "tcp_client.hpp"
# include "udp_client.hpp"
# define WIN_XA 640
# define WIN_YA 640
# include <chrono>
int main() {
	mdl::x11_window x11_window;
	mdl::tcp_client tcp_client;
	mdl::udp_client udp_client;
	tcp_client.init("192.168.0.100", 21298);
	if (udp_client.init("192.168.0.100", 21299)) while(1) {};

	x11_window.begin(WIN_XA, WIN_YA, "Firefly Engine");
	mdl::uint_t pix_count = (WIN_XA * WIN_YA) * 4;

	std::size_t tick_count = 0;
	auto begin = std::chrono::high_resolution_clock::now();

	while(true) {
		if (x11_window.pixels == nullptr) continue;
		tcp_client.write(x11_window.key_code);
		udp_client.recv(x11_window.pixels, pix_count);

		for (std::size_t y = 0; y != 64; y ++) {
			for (std::size_t x = 0; x != (WIN_XA * 4); x ++) {
				x11_window.pixels[x + (y * (WIN_XA * 4) )] = 255;
			}
		}
	}


		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(now - begin);
		std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		if (time_span.count() >= 1000) {
			std::cout << "FPS: " << tick_count << std::endl;
			tick_count = 0;

			begin = std::chrono::high_resolution_clock::now();
		} else tick_count ++;
	}

}*/
