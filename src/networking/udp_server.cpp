# include "udp_server.hpp"
/*
NOTE: take a look at the BSD socket docs to see what func take a refrence for the sock
*/

boost::int8_t mdl::firefly::networking::udp_server::init(boost::uint16_t __port_num) {
	this-> sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (this-> sock == -1) return -1;

	this-> len = sizeof(this-> serveraddr);
	bzero(&this-> serveraddr, len);

	this-> serveraddr.sin_family = AF_INET;
    this-> serveraddr.sin_addr.s_addr = htons(INADDR_ANY);
    this-> serveraddr.sin_port = htons(__port_num);

	if (bind(this-> sock, (struct sockaddr*)&this-> serveraddr, this-> len) == -1) return -1;

	this-> clientlen = sizeof(struct sockaddr_in);

	if (setsockopt(this-> sock, SOL_SOCKET, SO_SNDBUF, &this-> wbuff_size, sizeof(int)) == -1) {
		printf("failed to set send buffer size.\n");
		return -1;
	}

	if (setsockopt(this-> sock, SOL_SOCKET, SO_RCVBUF, &this-> rbuff_size, sizeof(int)) == -1) {
		printf("failed to set recv buffer size.\n");
		return -1;
	}

	struct timeval tv = {
		.tv_sec = 20,
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

	int optval = 1;
	if (setsockopt(this-> sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0) {
		return -1;
	}

	return 0;
}

boost::int8_t mdl::firefly::networking::udp_server::send_ack(struct sockaddr_in& __clientaddr) {
	boost::uint8_t null_val = 0;
	int_t sock_result = 0;

	sock_result = sendto(this-> sock, &null_val, 1, NULL, (struct sockaddr*)&__clientaddr, this-> clientlen);

	if (sock_result >= 0) {
		if (sock_result < 1) {
			fprintf(stderr, "failed to send ack to client. sock result: %d, errno: %d\n", sock_result, errno);
			return -1;
		}
	} else return sock_result;

	return 0;
}

boost::int8_t mdl::firefly::networking::udp_server::recv_ack(struct sockaddr_in& __clientaddr) {
	boost::uint8_t null_val = 0;
	int_t sock_result = 0;

	sock_result = recvfrom(this-> sock, &null_val, 1, NULL, (struct sockaddr*)&__clientaddr, &this-> clientlen);

	if (sock_result >= 0) {
		if (sock_result < 1) {
			fprintf(stderr, "failed to recv ack from client. sock result: %d, errno: %d\n", sock_result, errno);
			return -1;
		}
	} else return sock_result;

	return 0;
}

boost::int8_t mdl::firefly::networking::udp_server::send(boost::uint8_t *__buff, uint_t __buff_len, struct sockaddr_in& __clientaddr) {
	std::size_t amount_of_packets = ceil(float(__buff_len) / PACKET_SIZE);
    uint_t amount_to_send = __buff_len;

    std::size_t o = 0;
	int_t sock_result = 0;
    while (o != amount_of_packets) {
		if (this-> recv_ack(__clientaddr) == -1) return -1;

		uint_t bytes_to_send = PACKET_SIZE;

		if (amount_to_send < PACKET_SIZE)
			bytes_to_send = amount_to_send;

		sock_result = sendto(this-> sock, __buff + (o * PACKET_SIZE), bytes_to_send, NULL, (struct sockaddr*)&__clientaddr, this-> clientlen);

		if (sock_result >= 0) {
			if (sock_result < bytes_to_send) {
				fprintf(stderr, "failed to send packet to client, sock result: %d, errno: %d\n", sock_result, errno);
				fprintf(stderr, "packet failed to send at stage: %d and %d bytes where meant to be sent.\n", o, bytes_to_send);
				return -1;
			}
		} else return sock_result;

		amount_to_send -= PACKET_SIZE;
		o ++;
    }
}

boost::int8_t mdl::firefly::networking::udp_server::recv(boost::uint8_t *__buff, uint_t __buff_len, struct sockaddr_in& __clientaddr) {
	std::size_t amount_of_packets = ceil(float(__buff_len) / PACKET_SIZE);
    uint_t amount_to_recv = __buff_len;

    std::size_t o = 0;
	int_t sock_result = 0;
    while (o != amount_of_packets) {
		uint_t bytes_to_recv = PACKET_SIZE;

		if (amount_to_recv < PACKET_SIZE)
			bytes_to_recv = amount_to_recv;

		sock_result = recvfrom(this-> sock, __buff + (o * PACKET_SIZE), bytes_to_recv, NULL, (struct sockaddr*)&__clientaddr, &this-> clientlen);
		if (sock_result >= 0) {
			if (sock_result < bytes_to_recv) {
				fprintf(stderr, "failed to recv packet from client, sock result: %d, errno: %d\n", sock_result, errno);
				fprintf(stderr, "packet failed to recv at stage: %d and %d bytes where meant to be received.\n", o, bytes_to_recv);
				return -1;
			}
		} else return sock_result;

		if (this-> send_ack(__clientaddr) == -1) return -1;

		amount_to_recv -= PACKET_SIZE;
		o ++;
    }
}
