# include "uni_worker.hpp"
//static bool *_is_running;

static bool *shutdown_queued = nullptr;

void mdl::firefly::uni_worker::handle_player(uint_t __player_id) {
	printf("player has entered uni chunk.\n");
	types::player_info_t& player_info = this-> player_index[__player_id].first;
	types::worker_config_t& worker_config = *this-> _worker_config;
	do {
		if (this-> shutdown_queued) break;

		while (!this-> can_handle) {
			if (this-> shutdown_queued) break;
			if (!this-> is_running) break;
		}

		//printf("player-x: %d, player-y: %d\n", player_info.xaxis, player_info.yaxis);
		for (std::size_t y = player_info.yaxis; y != player_info.yaxis + 16; y ++) {
			for (std::size_t x = player_info.xaxis; x != player_info.xaxis + 16; x ++) {
				uint_t point = (x + (y * worker_config.chunk_xlen)) * 4;
				this-> uni_chunk[point] = 255;
				this-> uni_chunk[point + 1] = 0;
				this-> uni_chunk[point + 2] = 0;
				this-> uni_chunk[point + 3] = 100;
			}
		}
		this-> handle_done = true;

	} while(this-> is_running);
	printf("player has left uni chunk.\n");
}

boost::int8_t mdl::firefly::uni_worker::begin(char const *__server_ipaddr) {
	mdl::firefly::networking::tcp_client tcp_stream;
	mdl::firefly::networking::udp_client udp_stream;

	types::worker_config_t worker_config = {
		.chunk_xaxis = NULL,
		.chunk_yaxis = NULL,
		.chunk_zaxis = NULL,
		.chunk_xlen = NULL,
		.chunk_ylen = NULL,
		.chunk_zlen = NULL
	};

	this-> _worker_config = &worker_config;

	uint_t in_range_players = 0;
	serializer serialize('\0');

	int sock;

	tcp_stream.begin_poll(1000000000);

	types::player_info_t dummy_player_info;
	std::size_t pi_size = serialize.get_size(&dummy_player_info);
	std::size_t cf_size = serialize.get_size(&worker_config);
	do {
		if (this-> shutdown_queued == true) break;

		mdl::int_t sock_result = 0;
		if (!this-> server_connection) {
			if (tcp_stream.init(__server_ipaddr, 21298, sock) == -1) {
				this-> server_connection = false;
				fprintf(stderr, "failed to connect to server. attempt: %d\n", this-> connect_trys);
				goto skip;
			}

			if (udp_stream.init(__server_ipaddr, 10197) == -1) {
				this-> server_connection = false;
				fprintf(stderr, "failed to connect to server. attempt: %d\n", this-> connect_trys);
				goto skip;
			}

			if (tcp_stream.connect() == -1) {
				this-> server_connection = false;
				fprintf(stderr, "failed to connect to server. attempt: %d\n", this-> connect_trys);
				goto skip;
			}

			serialize.init(cf_size);
			serialize | 'w';

			tcp_stream.recv(serialize.get_serial(), cf_size);
			worker_config.achieve(serialize);
			serialize.reset();

			serialize.init(pi_size);
			serialize | 'w';

			if (this-> uni_chunk != nullptr) {
				memory::mem_free(this-> uni_chunk);
			}

			this-> uni_chunk = memory::alloc_pixmap(worker_config.chunk_xlen, worker_config.chunk_ylen, worker_config.chunk_zlen);
			memset(this-> uni_chunk, 166, (worker_config.chunk_xlen * worker_config.chunk_ylen * worker_config.chunk_zlen) * 4);

			this-> uni_chunk_size = (worker_config.chunk_xlen * worker_config.chunk_ylen * worker_config.chunk_zlen) * 4;

			printf("chunk xlen: %d, chunk ylen: %d, chunk zlen: %d\n", worker_config.chunk_xlen, worker_config.chunk_ylen, worker_config.chunk_zlen);
			printf("chunk xaxis: %d, chunk yaxis: %d, chunk zaxis: %d\n", worker_config.chunk_xaxis, worker_config.chunk_yaxis, worker_config.chunk_zaxis);

			this-> server_connection = true;
		}

		sock_result = tcp_stream.recv(in_range_players);
		if (sock_result == -1) {
			printf("0, rr: %d\n", in_range_players);
			this-> clean_up();
			return -1;
		}

		if (player_index.size() != in_range_players) {
			player_index.resize(in_range_players);
		}

		if (in_range_players == 0) continue;

		for (std::size_t o = 0; o != in_range_players; o ++) {
			sock_result = tcp_stream.recv(serialize.get_serial(), pi_size);
			if (sock_result == -1) {
				printf("1\n");
				this-> clean_up();
				return -1;
			}

			player_index[o].first.xaxis = 0;
			player_index[o].first.yaxis = 0;
			player_index[o].first.zaxis = 0;
			player_index[o].first.achieve(serialize);

			if (!player_index[o].second) {
				boost::thread(boost::bind(&uni_worker::handle_player, this, o));
				player_index[o].second = true;
			}

			serialize.reset();
		}

		this-> can_handle = true;

		while(!this-> handle_done) {
			if (this-> shutdown_queued) break;
			if (!this-> is_running) break;
		}

		this-> handle_done = false;
		this-> can_handle = false;

		sock_result = udp_stream.send(this-> uni_chunk, this-> uni_chunk_size);
		if (sock_result == -1 || sock_result == 0) {
			printf("2\n");
			this-> clean_up();
			return -1;
		}

		skip:
		if (!this-> server_connection) {
			if (connect_trys == FFLY_MAX_CONN_TRYS - 1) return -1;
			else connect_trys ++;
			usleep(FFLY_DELAY_BT_TRYS);
			continue;
		}

		in_range_players = 0;
	} while(this-> is_running);
	close(sock);
}

void ctrl_c(int sig) {
	if (shutdown_queued == nullptr) {
		fprintf(stderr, "the ctrl_c was to fast. init has not been called yet, try later.");
		return;
	}

	printf("looks like ctrl_c was called bye bye...\n");
	*shutdown_queued = true;
}

boost::int8_t mdl::firefly::uni_worker::init() {
	struct sigaction sig_handler;
	sig_handler.sa_handler = ctrl_c;

	sigemptyset(&sig_handler.sa_mask);

	sig_handler.sa_flags = 0;

	sigaction(SIGINT, &sig_handler, NULL);

	this-> is_running = true;
	this-> shutdown_queued = false;
	::shutdown_queued = &this-> shutdown_queued;
}

int main() {
	mdl::firefly::uni_worker uni_worker;
	uni_worker.init();
	uni_worker.begin("192.168.0.100");
	printf("exiting.\n");
}
