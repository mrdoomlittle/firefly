# include "tcp_server.hpp"
bool mdl::firefly::networking::tcp_server::to_shutdown = false;
mdl::firefly::types::err_t mdl::firefly::networking::tcp_server::init(boost::uint16_t __port_no) {
	this-> sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this-> sock == -1) {
		fprintf(stderr, "tcp_server: failed to create sock, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	this-> server_addr.sin_family = AF_INET;
	this-> server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	this-> server_addr.sin_port = htons(__port_no);

	if (bind(this-> sock, (struct sockaddr*)&this-> server_addr, sizeof(this-> server_addr)) == -1) {
		fprintf(stderr, "tcp_server: failed to bind sock, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	this-> poll_th_c = 0;
	len = sizeof(struct sockaddr_in);
/*
	struct timeval tv = {
		.tv_sec = 2,
		.tv_usec = 0
	};

	if (setsockopt(this-> sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval)) < 0) {
		printf("failed to set send time out.\n");
		return -1;
	}

	if (setsockopt(this-> sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)) < 0) {
		printf("failed to set recv time out.\n");
		return -1;
	}
*/
	this-> can_poll = true;
	this-> poll_lock = true;

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::networking::tcp_server::de_init() {
	tcp_server::to_shutdown = true;
	while(this-> poll_th_c != 0) {}
	return FFLY_SUCCESS;
}

void mdl::firefly::networking::tcp_server::poll_handler(int __sock, boost::int8_t *__sock_state, uint_t const __delay_ns) {
	*__sock_state = SOCK_ALIVE;

	std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point end;
	this-> poll_th_c++;
	do {
		types::byte_t null_val = 0;

		while (!this-> can_poll) {
			this-> poll_lock = false;
			begin = std::chrono::high_resolution_clock::now();
		}

		end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
		if (duration.count() < __delay_ns) continue;

		int_t sock_result;
		if ((sock_result = ::recv(__sock, &null_val, sizeof(boost::uint8_t), NULL)) == FFLY_FAILURE) {
			*__sock_state = SOCK_DEAD;
		} else {
			*__sock_state = SOCK_ALIVE;
			printf("sock result: %d, value: %d\n", sock_result, null_val);
		}

		begin = std::chrono::high_resolution_clock::now();
	} while(!tcp_server::to_shutdown);
	this-> poll_th_c--;
}

boost::int8_t* mdl::firefly::networking::tcp_server::begin_poll(int __sock, uint_t const __delay_ns) {
	boost::int8_t *sock_state = (boost::int8_t *)malloc(sizeof(boost::int8_t));
	boost::thread(boost::bind(&tcp_server::poll_handler, this, __sock, sock_state, __delay_ns));
	this-> polling_enabled = true;
	this-> poll_sock.insert(__sock);

	return sock_state;
}

mdl::firefly::types::err_t mdl::firefly::networking::tcp_server::listen() {
	if (::listen(this-> sock, 20) == -1) return FFLY_FAILURE;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::networking::tcp_server::accept(int& __sock) {
	if ((__sock = ::accept(this-> sock, (struct sockaddr*)&this-> client_addr, &this-> len)) == -1)
		return FFLY_FAILURE;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::networking::tcp_server::send(int __sock, types::byte_t *__buff, uint_t __buff_len) {
	if (::send(__sock, __buff, __buff_len, 0) == -1)
		return FFLY_FAILURE;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::networking::tcp_server::recv(int __sock, types::byte_t *__buff, uint_t __buff_len) {
	bool is_sock = false;

	if (this-> poll_sock.find(__sock) != this-> poll_sock.end()) {
		is_sock = true;
	}

	if (this-> polling_enabled && is_sock) {
		this-> can_poll = false;
		while (this-> poll_lock) {}
	}

	boost::int8_t sock_result = ::recv(__sock, __buff, __buff_len, 0);

	if (this-> polling_enabled && is_sock) {
		this-> can_poll = true;
		this-> poll_lock = true;
	}

	if (sock_result == -1) return FFLY_FAILURE;
	return FFLY_SUCCESS;
}
