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
#include <signal.h>

# define BUFFSIZE 1024
static const pa_sample_spec sample_spec = {
	.format = PA_SAMPLE_S16LE,
	.rate = 44100,
	.channels = 2
};

boost::uint8_t * to_free[2];
void clt_c(int sig) {
	printf("clt c : called\n");
	if (to_free[0] != nullptr) std::free(to_free[0]);
	if (to_free[1] != nullptr) std::free(to_free[1]);
	exit(1);
}

int unsigned wxy[2] = {0, 0};
bool connected = false;
bool connect_to_server = false;
int sock = -1;
bool exit_called = false;
boost::uint8_t *p = nullptr;
void btn_exit_func(int id) {
	exit_called = true;
	printf("exiting server.\n");
	connected = false;
	connect_to_server = false;
	if (p != nullptr) {
		reset_pixmap(p, wxy);
		printf("resetting background. %dx%d\n", wxy[0], wxy[1]);
	}
	if (sock != -1) close(sock);
}

mdl::ffly_client::ffly_client(uint_t __window_xlen, uint_t __window_ylen)
	: window_xlen(__window_xlen), window_ylen(__window_ylen) {

}

void make_s() {
	char const *argv[] = {"./client.exec", "test.wav"};
	pa_simple *sample = NULL;
	int any_error = 0, fd;

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
}

void test_btn_press(int id) {
	make_s();
	connect_to_server = true;
	printf("button was pressed.\n");
}

void hover() {
	//make_s();
	boost::thread t{make_s};
	printf("hovering.\n");
}

boost::uint8_t mdl::ffly_client::begin(char const * __frame_title) {
	struct sigaction sig_handler;

	sig_handler.sa_handler = clt_c;
	sigemptyset(&sig_handler.sa_mask);
	sig_handler.sa_flags = 0;

	sigaction(SIGINT, &sig_handler, NULL);

	mdl::x11_window _x11_window;
	_x11_window.begin(this-> window_xlen, this-> window_ylen, __frame_title);


	wxy[0] = this-> window_xlen;
	wxy[1] = this-> window_ylen;

	int unsigned pix_count = (this-> window_xlen * this-> window_ylen) * 4;
	boost::uint8_t *pixmap = nullptr, *hpixmap = nullptr;
	int unsigned pixmap_size[2] = {0};

	to_free[0] = pixmap;
	to_free[1] = hpixmap;

	boost::uint8_t *ebtn_pixmap = nullptr;
	int unsigned ebtn_pixmap_size[2] = {0};

	if (load_png_file("button.png", pixmap, pixmap_size)) return 1;
	if (load_png_file("hbutton.png", hpixmap, pixmap_size)) return 1;
	if (load_png_file("btn_exit.png", ebtn_pixmap, ebtn_pixmap_size)) return 1;
	if (pixmap == nullptr || hpixmap == nullptr || ebtn_pixmap == nullptr) {
		printf("somthing is not right...\n");
	}//return 1;

	

//	if (pixmap[0] == hpixmap[0]) return 1;

//	printf("pixmap size: %dx%d\n", pixmap_size[0], pixmap_size[1]);

	while(_x11_window.pixels == nullptr) {}
	mdl::client_gui _client_gui(_x11_window.pixels, &_x11_window.mouse_coords, &_x11_window.button_press, &_x11_window.button_code, this-> window_xlen, this-> window_ylen);

	mdl::uint_t btn_connect = _client_gui.create_button(pixmap, hpixmap, &hover, &test_btn_press, 46, 46, pixmap_size[0], pixmap_size[1]);
	mdl::uint_t btn_exit = _client_gui.create_button(ebtn_pixmap, nullptr, nullptr, &btn_exit_func, 46, 46, ebtn_pixmap_size[0], ebtn_pixmap_size[1]);
	_client_gui.btn_disable(btn_exit);
	_client_gui.unlock_drawing(btn_exit);
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
	tcp_client _tcp_stream;
//	_tcp_stream.init("192.168.0.100", 21299);
//	_tcp_stream.connect();


	udp_client _udp_stream;
//	_udp_stream.init("192.168.0.100", 10198);

	//bool connected = false;
	p = _x11_window.pixels;

	bool connection_error = false;
	do {
		while(!_x11_window.waitting) {}
		while(_x11_window.done_drawing) {}
		//printf("connected: %d, connection_error: %d\n", connected, connection_error);

		if (connected)
		{
	//		printf("hello\n");
			if (_tcp_stream.send(_x11_window.key_code) == -1) connection_error = true;

			if (!connection_error) {
				if (_udp_stream.recv(_x11_window.pixels, pix_count) == -1) connection_error = true;
			}

			if (connection_error) {
				printf("connection error.\n");
		//		reset_pixmap(_x11_window.pixels, wxy);
				connected = false;
				connect_to_server = false;
				_client_gui.btn_enable(btn_connect);
                _client_gui.btn_disable(btn_exit);
			}
			connect_to_server = true;
		}

		_client_gui.handle_buttons();
		if (exit_called) {
			_client_gui.btn_enable(btn_connect);
			_client_gui.btn_disable(btn_exit);
			exit_called = false;
		}

		if (connect_to_server && !connected)
		{
			_client_gui.btn_disable(btn_connect);
			_client_gui.btn_enable(btn_exit);

			printf("connecting to the network.\n");

			if (_tcp_stream.init("192.168.0.100", 21299, sock) == -1) {
				connected = false;
				printf("error initing tcp stream.\n");
				goto end;
			}

			printf("initing tcp stream.\n");

			if (_tcp_stream.connect() == -1) {
				connected = false;
				printf("error connecting to tcp stream.\n");
				goto end;
			}

			printf("connected to tcp stream.\n");

			if (_udp_stream.init("192.168.0.100", 10198) == -1) {
				connected = false;
				printf("error initing udp stream.\n");
				goto end;
			}

			printf("initing udp stream.\n");

			connected = true;

			end:

			if (!connected) {
				printf("there might of been an error connecting.\n");
				_client_gui.btn_enable(btn_connect);
				_client_gui.btn_disable(btn_exit);
				connect_to_server = false;
			}
			//connect_to_server = false;
		}

		//if (!connected) printf("look like the server it not working. oh no...\n");

		_x11_window.done_drawing = true;

		if (_x11_window.button_press) printf("%d\n", _x11_window.button_code);
		//usleep(10000);
		_x11_window.waitting = false;
	} while(1);

	printf("exiting.\n");
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
