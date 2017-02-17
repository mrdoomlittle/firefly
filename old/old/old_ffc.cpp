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
# include <chrono>
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

# include "graphics/text.hpp"
boost::uint8_t mdl::ffly_client::begin(char const * __frame_title, void (* __o)(boost::int8_t, portal_t*)) {
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

	while(_x11_window.pixels == nullptr) {}
	mdl::client_gui _client_gui(_x11_window.pixels, &_x11_window.mouse_coords, &_x11_window.button_press, &_x11_window.button_code, this-> window_xlen, this-> window_ylen);

	mdl::uint_t btn_connect = _client_gui.create_button(pixmap, hpixmap, &hover, &test_btn_press, 46, 46, pixmap_size[0], pixmap_size[1]);
	mdl::uint_t btn_exit = _client_gui.create_button(ebtn_pixmap, nullptr, nullptr, &btn_exit_func, 46, 46, ebtn_pixmap_size[0], ebtn_pixmap_size[1]);
	_client_gui.btn_disable(btn_exit);
	_client_gui.unlock_drawing(btn_exit);

	tcp_client _tcp_stream;
	udp_client _udp_stream;

	p = _x11_window.pixels;

	this-> portal.pixbuff = _x11_window.pixels;
	this-> portal._this = this;
	bool connection_error = false;
	//text tx;

	auto begin = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();

	do {
		if (_x11_window.window_closed && pthread_kill(_x11_window.window_thread-> native_handle(), 0) != 0) {
			printf("window has been closed.\n");
			break;
		}

		if (!_x11_window.waitting) continue;
		if (_x11_window.done_drawing) continue;

		goto le;
		if (connected)
		{
			if (_tcp_stream.send(_x11_window.key_code) == -1) connection_error = true;

			if (!connection_error) {
				if (_udp_stream.recv(_x11_window.pixels, pix_count) == -1) connection_error = true;
			}

			if (connection_error) {
				printf("connection error.\n");
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

		le:

		__o(0, &this-> portal);
		if (_x11_window.button_press) printf("%d\n", _x11_window.button_code);

		_x11_window.done_drawing = true;
		_x11_window.waitting = false;

		end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
		if (duration.count() >= 1000) {
			begin = std::chrono::high_resolution_clock::now();
			this-> curr_fps = fps_counter;
			this-> fps_counter = 0;
		} else this-> fps_counter ++;
	} while(1);

	printf("engine is closing.\n");
}
