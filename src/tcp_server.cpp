# include "tcp_server.hpp"
boost::uint8_t mdl::tcp_server::init(boost::uint16_t __port_num) {
	this-> sock = socket(AF_INET, SOCK_STREAM, 0);

	this-> serveraddr.sin_family = AF_INET;
	this-> serveraddr.sin_addr.s_addr = htons(INADDR_ANY);
	this-> serveraddr.sin_port = htons(__port_num);
	this-> len = sizeof(this-> serveraddr);

	bind(this-> sock, (struct sockaddr*)&this-> serveraddr, this-> len);
	listen(this-> sock, 20);
	len = sizeof(this-> clientaddr);

	return 0;
}

void mdl::tcp_server::accept(int& __sock) {
	__sock = ::accept(this-> sock, (struct sockaddr*)&this-> clientaddr, &this-> len);
}

void mdl::tcp_server::send(int __sock, boost::uint8_t *__buff, uint_t __buff_len) {
	::send(__sock, __buff, __buff_len, 0);
}

void mdl::tcp_server::recv(int __sock, boost::uint8_t *__buff, uint_t __buff_len) {
	::recv(__sock, __buff, __buff_len, 0);
}
/*
int main() {
	mdl::tcp_server tcp_server;
	tcp_server.init(21299);
}*/
