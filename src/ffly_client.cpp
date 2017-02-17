# include "ffly_client.hpp"
boost::uint8_t mdl::ffly_client::begin(char const * __frame_title, void (* __extern_loop)(boost::int8_t, portal_t *)) {
	x11_window _x11_window;
	_x11_window.begin(this-> win_xlen, this-> win_ylen, __frame_title);

	while(_x11_window.pixels == nullptr) {}

	auto begin = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	this-> portal.pixbuff = _x11_window.pixels;
	this-> portal._this = this;

	uint_t pix_count = (this-> win_xlen * this-> win_ylen) * 4;

	int sock;
	firefly::types::player_info_t player_info;
	serial serialize('\0');

	std::size_t ss = serialize.get_size(&player_info);
	serialize.init(ss);

	serialize | 'r';

	do {
		if (_x11_window.window_closed && pthread_kill(_x11_window.window_thread-> native_handle(), 0) != 0) {
			printf("window has been closed.\n");
			break;
		}

		if (!_x11_window.waitting) continue;
		if (_x11_window.done_drawing) continue;

		player_info.key_code = _x11_window.key_code;

		if (this-> server_connected) {
			player_info.achieve(serialize);
			serialize.reset();
			if (this-> tcp_stream.send(serialize.get_data(), ss) == -1) {
				this-> server_connected = false;
				fprintf(stderr, "connection error.\n");
				goto sskip;
			}

			if (this-> udp_stream.recv(_x11_window.pixels, pix_count) == -1) {
				this-> server_connected = false;
				fprintf(stderr, "connection error.\n");
				goto sskip;
			}
		}
		sskip:

		__extern_loop(0, &this-> portal);

		if (this-> server_ipaddr != nullptr && !this-> server_connected) {
			if (this-> tcp_stream.init(this-> server_ipaddr, 21299, sock) == -1) {
				this-> server_connected = false;
				fprintf(stderr, "can't connected to server.\n");
				goto skip;
			}

			if (this-> tcp_stream.connect() == -1) {
				this-> server_connected = false;
				fprintf(stderr, "can't connect to server.\n");
				goto skip;
			}

			if (this-> udp_stream.init(this-> server_ipaddr, 10198) == -1) {
				this-> server_connected = false;
				fprintf(stderr, "cant't connect to server.\n");
				goto skip;
			}

			this-> server_connected = true;
		}

		skip:

		_x11_window.done_drawing = true;
		_x11_window.waitting = false;

		end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

		if (duration.count() >= 1000) {
			begin = std::chrono::high_resolution_clock::now();
			this-> curr_fps = fps_counter;
			this-> fps_counter = 0;
		} else this-> fps_counter++;

	} while (true);
}
