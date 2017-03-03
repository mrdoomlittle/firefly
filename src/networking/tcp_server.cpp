# include "tcp_server.hpp"
boost::int8_t mdl::firefly::networking::tcp_server::init(boost::uint16_t __port_num) {
	this-> sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this-> sock == -1) return -1;

	this-> serveraddr.sin_family = AF_INET;
	this-> serveraddr.sin_addr.s_addr = htons(INADDR_ANY);
	this-> serveraddr.sin_port = htons(__port_num);
	this-> len = sizeof(this-> serveraddr);

	if (bind(this-> sock, (struct sockaddr*)&this-> serveraddr, this-> len) == -1) return -1;

	len = sizeof(this-> clientaddr);
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

	return 0;
}

void mdl::firefly::networking::tcp_server::poll_handler(int __sock, boost::int8_t *__sock_state, uint_t const __delay_ns) {
	*__sock_state = SOCK_ALIVE;

	auto begin = std::chrono::high_resolution_clock::now();
	do {
		boost::uint8_t null_val = 0;

		while (!this-> can_poll) {
			this-> poll_lock = false;
			begin = std::chrono::high_resolution_clock::now();
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
		if (duration.count() < __delay_ns) continue;

		int_t sock_result;
		if ((sock_result = ::recv(__sock, &null_val, sizeof(boost::uint8_t), NULL)) == 0) {
			*__sock_state = SOCK_DEAD;
		} else {
			*__sock_state = SOCK_ALIVE;
			printf("sock result: %d, value: %d\n", sock_result, null_val);
		}

		begin = std::chrono::high_resolution_clock::now();
	} while(true);
}
static int temp = 0;
boost::int8_t* mdl::firefly::networking::tcp_server::begin_poll(int __sock, uint_t const __delay_ns) {
	boost::int8_t *sock_state = (boost::int8_t *)malloc(sizeof(boost::int8_t));
	boost::thread(boost::bind(&tcp_server::poll_handler, this, __sock, sock_state, __delay_ns));
	this-> polling_enabled = true;
	this-> poll_sock.insert(__sock);
	temp ++;
	return sock_state;
}

boost::int8_t mdl::firefly::networking::tcp_server::listen() {
	if (::listen(this-> sock, 20) == -1) return -1;
}

boost::int8_t mdl::firefly::networking::tcp_server::accept(int& __sock) {
	__sock = ::accept(this-> sock, (struct sockaddr*)&this-> clientaddr, &this-> len);
	if (__sock == -1) return -1;
}

boost::int8_t mdl::firefly::networking::tcp_server::send(int __sock, boost::uint8_t *__buff, uint_t __buff_len) {
	return ::send(__sock, __buff, __buff_len, 0);
}

boost::int8_t mdl::firefly::networking::tcp_server::recv(int __sock, boost::uint8_t *__buff, uint_t __buff_len) {
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

	return sock_result;
}
