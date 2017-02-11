
# include "udp_client.hpp"
boost::uint8_t mdl::udp_client::init(char const * __ip_addr, boost::uint16_t __port_num) {
	this-> sock_len = sizeof(struct sockaddr_in);
	this-> sock = socket(AF_INET, SOCK_DGRAM, 0);

	this-> serveraddr.sin_family = AF_INET;
	this-> serveraddr.sin_port = htons(__port_num);
	inet_pton(AF_INET, __ip_addr, &(this-> serveraddr.sin_addr));

	if (setsockopt(this-> sock, SOL_SOCKET, SO_SNDBUF, &this-> wbuff_size, sizeof(int)) == -1) {
		printf("failed to set send buffer size.\n");
		return 1;
	}

	if (setsockopt(this-> sock, SOL_SOCKET, SO_RCVBUF, &this-> rbuff_size, sizeof(int)) == -1) {
		printf("failed to set recv buffer size.\n");
		return 1;
	}

	return 0;
}

void mdl::udp_client::send(boost::uint8_t *__buff, uint_t __buff_len) {
	std::size_t amount_of_packets = ceil(float(__buff_len) / PACKET_SIZE);
	uint_t amount_to_send = __buff_len;

	boost::uint8_t null_val = 0;
	std::size_t o = 0;
	while (o != amount_of_packets) {
		recvfrom(this-> sock, &null_val, 1, 0, (struct sockaddr*)&this-> serveraddr, &this-> sock_len);
		uint_t bytes_to_send = PACKET_SIZE;

		if (amount_to_send < PACKET_SIZE)
			bytes_to_send = amount_to_send;

		sendto(this-> sock, __buff + (o * PACKET_SIZE), bytes_to_send, 0, (struct sockaddr*)&this-> serveraddr, this-> sock_len);

		amount_to_send -= PACKET_SIZE;
        o ++;
	}
}

void mdl::udp_client::recv(boost::uint8_t *__buff, uint_t __buff_len) {
	std::size_t amount_of_packets = ceil(float(__buff_len) / PACKET_SIZE);
	uint_t amount_to_recv = __buff_len;

	boost::uint8_t null_val = 0;
	std::size_t o = 0;
	while (o != amount_of_packets) {
		sendto(this-> sock, &null_val, 1, 0, (struct sockaddr*)&this-> serveraddr, this-> sock_len);
		uint_t bytes_to_recv = PACKET_SIZE;

		if (amount_to_recv < PACKET_SIZE)
			bytes_to_recv = amount_to_recv;

		recvfrom(this-> sock, __buff + (o * PACKET_SIZE), bytes_to_recv, 0, (struct sockaddr*)&this-> serveraddr, &this-> sock_len);

		amount_to_recv -= PACKET_SIZE;
		o ++;
	}
}
