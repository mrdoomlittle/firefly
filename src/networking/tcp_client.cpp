# include "tcp_client.hpp"
boost::int8_t mdl::firefly::networking::tcp_client::init(char const * __ip_addr, boost::uint16_t __portno, int& __sock) {
	this-> sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this-> sock == -1) return -1;
	__sock = this-> sock;
	this-> serveraddr.sin_family = AF_INET;
	this-> serveraddr.sin_port = htons(__portno);
	inet_pton(AF_INET, __ip_addr, &(this-> serveraddr.sin_addr));
/*
	struct timeval tv {
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

void mdl::firefly::networking::tcp_client::poll_handler(uint_t __delay_ns) {
	boost::uint8_t null_val = 2;

	auto begin = std::chrono::high_resolution_clock::now();
	do {
		while (!this-> can_poll) {
			this-> poll_lock = false;
			begin = std::chrono::high_resolution_clock::now();
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
		if (duration.count() < __delay_ns) continue;

		printf("sending poll to server.\n");
		::send(this-> sock, &null_val, sizeof(boost::uint8_t), 0);

		begin = std::chrono::high_resolution_clock::now();
	} while(true);
}

void mdl::firefly::networking::tcp_client::begin_poll(uint_t __delay_ns) {
	boost::thread(boost::bind(&tcp_client::poll_handler, this, __delay_ns));
	this-> polling_enabled = true;
}

boost::int8_t mdl::firefly::networking::tcp_client::connect() {
	return ::connect(this-> sock, (struct sockaddr*)&this-> serveraddr, sizeof(this-> serveraddr));
}

boost::int8_t mdl::firefly::networking::tcp_client::send(boost::uint8_t *__buff, uint_t __buff_len) {
	if (this-> polling_enabled) {
		this-> can_poll = false;
		while (this-> poll_lock) {}
	}

	boost::int8_t sock_result = ::send(this-> sock, __buff, __buff_len, 0);

	if (this-> polling_enabled) {
		this-> can_poll = true;
		this-> poll_lock = true;
	}

	return sock_result;
}

boost::int8_t mdl::firefly::networking::tcp_client::recv(boost::uint8_t *__buff, uint_t __buff_len) {
	return ::recv(this-> sock, __buff, __buff_len, 0);
}
