# include "ffly_client.hpp"
static bool *_to_shutdown = nullptr;

boost::int8_t mdl::ffly_client::connect_to_server(int& __sock) {
	for (; this-> connect_trys != FFLY_MAX_CONN_TRYS; this-> connect_trys++) {
		if (this-> connect_trys != 0) {
			fprintf(stderr, "failed to connected to server, attempt: %d\n", this-> connect_trys);
			usleep(FFLY_DELAY_BT_TRYS);
		}

		this-> server_connected = true;

		if (this-> tcp_stream.init(this-> server_ipaddr, 21299, __sock) == FFLY_FAILURE) {
			this-> server_connected = false;
			fprintf(stderr, "failed to init tcp stream, errno: %d\n", errno);
			continue;
		}

		if (this-> tcp_stream.connect() == FFLY_FAILURE) {
			this-> server_connected = false;
			fprintf(stderr, "failed to connect to server thru tcp stream, errno: %d\n", errno);
			continue;
		}

		if (this-> udp_stream.init(this-> server_ipaddr, 10198) == FFLY_FAILURE) {
			this-> server_connected = false;
			fprintf(stderr, "failed to init udp stream, errno: %d\n",  errno);
			continue;
		}

		if (server_connected) {
			this-> connect_trys = 0;
			this-> server_connected = true;
			return FFLY_SUCCESS;
		}
	}

	return FFLY_FAILURE;
}

boost::int8_t mdl::ffly_client::send_client_info() {
	static serializer serialize('\0');
	static std::size_t size = serialize.get_size(&this-> client_info);
	static bool initialized = false;

	if (!initialized) {
		serialize.init(size);
		serialize | 'r';
	}

	this-> client_info.achieve(serialize);
	serialize.reset();

	if (this-> tcp_stream.send(serialize.get_serial(), size) == FFLY_FAILURE) {
		fprintf(stderr, "failed to send client info to server, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}

boost::int8_t mdl::ffly_client::recv_cam_frame() {
	if (this-> udp_stream.recv(this-> layer.get_layer_pixmap(this-> cam_layer_id), this-> cam_frame_size) == FFLY_FAILURE) {
		fprintf(stderr, "failed to recv camera feed from server, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}

void ctrl_c(int __sig) {
	printf("look like ctrl_c was called bye bye...\n");
	*_to_shutdown = true;
}

boost::int8_t mdl::ffly_client::init() {
	_to_shutdown = &this-> to_shutdown;

	struct sigaction sig_handler;

	sig_handler.sa_handler = ctrl_c;
	sigemptyset(&sig_handler.sa_mask);
	sig_handler.sa_flags = 0;
	sigaction(SIGINT, &sig_handler, NULL);
}

boost::uint8_t mdl::ffly_client::begin(char const * __frame_title, void (* __extern_loop)(boost::int8_t, portal_t *)) {
	firefly::graphics::x11_window _x11_window;
	_x11_window.begin(this-> win_xlen, this-> win_ylen, __frame_title);

	_x11_window.set_fps_mark(240);

	while(!_x11_window.is_wd_flag(X11_WD_OPEN)) {
		if (this-> to_shutdown) return 0;
	}

	auto begin = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	this-> portal.pixbuff = _x11_window.pixbuff;
	this-> portal._this = this;

	uint_t pix_count = (this-> cam_xaxis_len * this-> cam_yaxis_len) * 4;

	int sock;
	do {
		if (_x11_window.is_wd_flag(X11_WD_CLOSED)) {
			printf("window has been closed.\n");
			break;
		}

		if (!_x11_window.is_wd_flag(X11_WD_WAITING)) continue;
		if (_x11_window.is_wd_flag(X11_WD_DONE_DRAW)) continue;

		client_info.key_code = _x11_window.key_code;

		if (this-> server_connected) {
			if (this-> send_client_info() == FFLY_FAILURE) break;
			if (this-> recv_cam_frame() == FFLY_FAILURE) break;
		}

		__extern_loop(0, &this-> portal);

		if (this-> server_ipaddr != nullptr && !this-> server_connected) {
			if (this-> connect_to_server(sock) == -1) break;
		}

		skip:

		this-> layer.draw_layers(_x11_window.pixbuff, this-> win_xlen, this-> win_ylen);

		_x11_window.add_wd_flag(X11_WD_DONE_DRAW);
		_x11_window.rm_wd_flag(X11_WD_WAITING);

		end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

		if (duration.count() >= 1000) {
			begin = std::chrono::high_resolution_clock::now();
			this-> curr_fps = fps_counter;
			this-> fps_counter = 0;
		} else this-> fps_counter++;

	} while (!this-> to_shutdown);

	_x11_window.add_wd_flag(X11_WD_TO_CLOSE);

	while(!_x11_window.is_wd_flag(X11_WD_CLOSED)){}

	return 0;
}
