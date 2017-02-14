# include "tcp_client.hpp"
boost::int8_t mdl::tcp_client::init(char const * __ip_addr, boost::uint16_t __portno, int& __sock) {
	this-> sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this-> sock == -1) return -1;
	__sock = this-> sock;
	this-> serveraddr.sin_family = AF_INET;
	this-> serveraddr.sin_port = htons(__portno);
	inet_pton(AF_INET, __ip_addr, &(this-> serveraddr.sin_addr));

	struct timeval tv;

	tv.tv_sec = 2;
	tv.tv_usec = 0;

	if (setsockopt(this-> sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval)) < 0) {
		printf("failed to set send time out.\n");
		return -1;
	}

	if (setsockopt(this-> sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)) < 0) {
		printf("failed to set recv time out.\n");
		return -1;
	}

	return 0;
}

boost::int8_t mdl::tcp_client::connect() {
	return ::connect(this-> sock, (struct sockaddr*)&this-> serveraddr, sizeof(this-> serveraddr));
}

boost::int8_t mdl::tcp_client::send(boost::uint8_t *__buff, uint_t __buff_len) {
	return ::send(this-> sock, __buff, __buff_len, 0);
}

boost::int8_t mdl::tcp_client::recv(boost::uint8_t *__buff, uint_t __buff_len) {
	return ::recv(this-> sock, __buff, __buff_len, 0);
}
/*
int main() {
	mdl::tcp_client tcp_client;
	tcp_client.init("192.168.0.10", 21299);
}*/
