# include "udp_server.hpp"
boost::uint8_t mdl::udp_server::init(boost::uint16_t __port_num) {
	this-> sock = socket(AF_INET, SOCK_DGRAM, 0);
	this-> len = sizeof(this-> server);
	bzero(&this-> server, len);

	this-> server.sin_family = AF_INET;
    this-> server.sin_addr.s_addr = htons(INADDR_ANY);
    this-> server.sin_port = htons(__port_num);

	bind(this-> sock, (struct sockaddr*)&this-> server, this-> len);

	this-> clientlen = sizeof(struct sockaddr_in);
}

void mdl::udp_server::send(boost::uint8_t *__buff, uint_t __buff_len) {
	std::size_t amount_of_packets = ceil(float(__buff_len) / PACKET_SIZE);
    uint_t amount_to_send = __buff_len;

	boost::uint8_t null_val = 0;
    std::size_t o = 0;
    while (o != amount_of_packets) {
		recvfrom(this-> sock, &null_val, 1, 0, (struct sockaddr*)&this-> client, &this-> clientlen);
		uint_t bytes_to_send = PACKET_SIZE;

		if (amount_to_send < PACKET_SIZE)
			bytes_to_send = amount_to_send;

		sendto(this-> sock, __buff + (o * PACKET_SIZE), bytes_to_send, 0, (struct sockaddr*)&this-> client, this-> clientlen);

		amount_to_send -= PACKET_SIZE;
		o ++;
    }
}

void mdl::udp_server::recv(boost::uint8_t *__buff, uint_t __buff_len) {
	std::size_t amount_of_packets = ceil(float(__buff_len) / PACKET_SIZE);
    uint_t amount_to_recv = __buff_len;

	boost::uint8_t null_val = 0;
    std::size_t o = 0;
    while (o != amount_of_packets) {
		sendto(this-> sock, &null_val, 1, 0, (struct sockaddr*)&this-> client, this-> clientlen);
		uint_t bytes_to_recv = PACKET_SIZE;

		if (amount_to_recv < PACKET_SIZE)
			bytes_to_recv = amount_to_recv;

		recvfrom(this-> sock, __buff + (o * PACKET_SIZE), bytes_to_recv, 0, (struct sockaddr*)&this-> client, &this-> clientlen);

		amount_to_recv -= PACKET_SIZE;
		o ++;
    }
}
