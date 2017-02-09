# include "tcp_client.hpp"
boost::uint8_t mdl::tcp_client::init(char const * __ip_addr, boost::uint16_t __port_num) {
	this-> sock = socket(AF_INET, SOCK_STREAM, 0);

	this-> server.sin_family = AF_INET;
	this-> server.sin_port = htons(__port_num);
	inet_pton(AF_INET, __ip_addr, &(this-> server.sin_addr));

	this-> k = connect(this-> sock, (struct sockaddr*)&this-> server, sizeof(this-> server));
}

void mdl::tcp_client::write(boost::uint8_t *__buff, uint_t __buff_len) {
	send(this-> sock, __buff, __buff_len, 0);
}

void mdl::tcp_client::read(boost::uint8_t *__buff, uint_t __buff_len) {
	recv(this-> sock, __buff, __buff_len, 0);
}
/*
int main() {
	mdl::tcp_client tcp_client;
	tcp_client.init("192.168.0.10", 21299);
}*/
