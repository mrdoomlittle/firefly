# include "tcp_client.hpp"
bool mdl::firefly::networking::tcp_client::to_shutdown = false;
boost::int8_t mdl::firefly::networking::tcp_client::init(char const * __ip_addr, boost::uint16_t __port_no, int& __sock) {
	this-> sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this-> sock == -1) {
		fprintf(stderr, "tcp_client: failed to create sock, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	__sock = this-> sock;
	this-> serveraddr.sin_family = AF_INET;
	this-> serveraddr.sin_port = htons(__port_no);
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
	this-> poll_th_c = 0;
	this-> can_poll = true;
	this-> poll_lock = true;

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::networking::tcp_client::de_init() {
	tcp_client::to_shutdown = true;
	while(this-> poll_th_c != 0) {}
	return FFLY_SUCCESS;
}

void mdl::firefly::networking::tcp_client::poll_handler(uint_t __delay_ns) {
	types::byte_t null_val = 2;

	std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point end;
	this-> poll_th_c++;
	do {
		while (!this-> can_poll) {
			this-> poll_lock = false;
			begin = std::chrono::high_resolution_clock::now();
		}

		end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
		if (duration.count() < __delay_ns) continue;

		printf("sending poll to server.\n");
		::send(this-> sock, &null_val, sizeof(boost::uint8_t), 0);

		begin = std::chrono::high_resolution_clock::now();
	} while(!tcp_client::to_shutdown);
	this-> poll_th_c--;
}

void mdl::firefly::networking::tcp_client::begin_poll(uint_t __delay_ns) {
	boost::thread(boost::bind(&tcp_client::poll_handler, this, __delay_ns));
	this-> polling_enabled = true;
}

mdl::firefly::types::err_t mdl::firefly::networking::tcp_client::connect() {
	if (::connect(this-> sock, (struct sockaddr*)&this-> serveraddr, sizeof(this-> serveraddr)) == -1)
		return FFLY_FAILURE;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t  mdl::firefly::networking::tcp_client::send(types::byte_t *__buff, uint_t __buff_len) {
	if (this-> polling_enabled) {
		this-> can_poll = false;
		while (this-> poll_lock) {}
	}

	types::err_t sock_result = ::send(this-> sock, __buff, __buff_len, 0);

	if (this-> polling_enabled) {
		this-> can_poll = true;
		this-> poll_lock = true;
	}

	if (sock_result == FFLY_FAILURE)
		return FFLY_FAILURE;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::networking::tcp_client::recv(types::byte_t *__buff, uint_t __buff_len) {
	if (::recv(this-> sock, __buff, __buff_len, 0))
		return FFLY_FAILURE;
	return FFLY_SUCCESS;
}
