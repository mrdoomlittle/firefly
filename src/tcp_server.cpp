# include "tcp_server.hpp"
boost::uint8_t mdl::tcp_server::init(boost::uint16_t __port_num) {
	this-> sock = socket(AF_INET, SOCK_STREAM, 0);

	this-> server.sin_family = AF_INET;
	this-> server.sin_addr.s_addr = htons(INADDR_ANY);
	this-> server.sin_port = htons(__port_num);
	this-> len = sizeof(this-> server);

	bind(this-> sock, (struct sockaddr*)&this-> server, this-> len);
	listen(this-> sock, 20);
	len = sizeof(this-> client);
	this-> tsock = accept(this-> sock, (struct sockaddr*)&this-> client, &this-> len);
}

void mdl::tcp_server::write(boost::uint8_t *__buff, uint_t __buff_len) {
	send(this-> tsock, __buff, __buff_len, 0);
}

void mdl::tcp_server::read(boost::uint8_t *__buff, uint_t __buff_len) {
	recv(this-> tsock, __buff, __buff_len, 0);
}
/*
int main() {
	mdl::tcp_server tcp_server;
	tcp_server.init(21299);
}*/
