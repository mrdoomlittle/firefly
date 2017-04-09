
# include "udp_client.hpp"
mdl::firefly::types::err_t mdl::firefly::networking::udp_client::init(char const * __ip_addr, boost::uint16_t __port_no) {
	this-> sock_len = sizeof(struct sockaddr_in);
	this-> sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (this-> sock == -1) {
		fprintf(stderr, "udp_client: failed to create sock, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	this-> serveraddr.sin_family = AF_INET;
	this-> serveraddr.sin_port = htons(__port_no);
	inet_pton(AF_INET, __ip_addr, &(this-> serveraddr.sin_addr));

	if (setsockopt(this-> sock, SOL_SOCKET, SO_SNDBUF, &this-> wbuff_size, sizeof(int)) == -1) {
		printf("failed to set send buffer size.\n");
		return FFLY_FAILURE;
	}

	if (setsockopt(this-> sock, SOL_SOCKET, SO_RCVBUF, &this-> rbuff_size, sizeof(int)) == -1) {
		printf("failed to set recv buffer size.\n");
		return FFLY_FAILURE;
	}

	struct timeval tv = {
		.tv_sec = 20,
		.tv_usec = 0
	};

	if (setsockopt(this-> sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval)) == -1) {
		printf("failed to set send time out.\n");
		return FFLY_FAILURE;
	}

	if (setsockopt(this-> sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)) == -1) {
		printf("failed to set recv time out.\n");
		return FFLY_FAILURE;
	}

	int optval = 1;
	if (setsockopt(this-> sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) == -1) {
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::networking::udp_client::send_ack() {
	types::byte_t null_val = 0;
	int_t sock_result = sendto(this-> sock, &null_val, 1, 0, (struct sockaddr*)&this-> serveraddr, this-> sock_len);

	if (sock_result >= 0) {
		if (sock_result < 1) {
			fprintf(stderr, "failed to send ack to server. sock result: %d, errno: %d\n", sock_result, errno);
			return FFLY_FAILURE;
		}
	} else return FFLY_FAILURE;

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::networking::udp_client::recv_ack() {
	types::byte_t null_val = 0;
	int_t sock_result = recvfrom(this-> sock, &null_val, 1, NULL, (struct sockaddr*)&this-> serveraddr, &this-> sock_len);

	if (sock_result >= 0) {
		if (sock_result < 1) {
			fprintf(stderr, "failed to recv ack from server. sock result: %d, errno: %d\n", sock_result, errno);
			return FFLY_FAILURE;
		}
	} else FFLY_FAILURE;

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::networking::udp_client::send(types::byte_t *__buff, uint_t __buff_len) {
	std::size_t amount_of_packets = (std::size_t)ceil(float(__buff_len) / PACKET_SIZE);
	uint_t amount_to_send = __buff_len;

	std::size_t o = 0;
	int_t sock_result = 0;
	while (o != amount_of_packets) {
		uint_t bytes_to_send = PACKET_SIZE;

		if (amount_to_send < PACKET_SIZE)
			bytes_to_send = amount_to_send;

		sock_result = sendto(this-> sock, __buff + (o * PACKET_SIZE), bytes_to_send, NULL, (struct sockaddr*)&this-> serveraddr, this-> sock_len);

		if (sock_result >= 0) {
			if (sock_result < bytes_to_send) {
				fprintf(stderr, "failed to send packet to server, sock result: %d, errno: %d\n", sock_result, errno);
				fprintf(stderr, "packet failed to send at stage: %d and %d bytes where meant to be sent.\n", o, bytes_to_send);
				return FFLY_FAILURE;
			}
		} else return FFLY_FAILURE;

		if (this-> recv_ack() == FFLY_FAILURE) return FFLY_FAILURE;

		amount_to_send -= PACKET_SIZE;
        o ++;
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::networking::udp_client::recv(types::byte_t *__buff, uint_t __buff_len) {
	std::size_t amount_of_packets = (std::size_t)ceil(float(__buff_len) / PACKET_SIZE);
	uint_t amount_to_recv = __buff_len;

	std::size_t o = 0;
	int_t sock_result = 0;
	while (o != amount_of_packets) {
		if (this-> send_ack() == FFLY_FAILURE) return FFLY_FAILURE;

		uint_t bytes_to_recv = PACKET_SIZE;

		if (amount_to_recv < PACKET_SIZE)
			bytes_to_recv = amount_to_recv;

		sock_result = recvfrom(this-> sock, __buff + (o * PACKET_SIZE), bytes_to_recv, NULL, (struct sockaddr*)&this-> serveraddr, &this-> sock_len);
		if (sock_result >= 0) {
			if (sock_result < bytes_to_recv) {
				fprintf(stderr, "failed to recv packet from server, sock result: %d, errno: %d\n", sock_result, errno);
				fprintf(stderr, "packet failed to recv at stage: %d and %d bytes where meant to be received.\n", o, bytes_to_recv);
				return FFLY_FAILURE;
			}
		} else return FFLY_FAILURE;

		amount_to_recv -= PACKET_SIZE;
		o ++;
	}
	return FFLY_SUCCESS;
}
