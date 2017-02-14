# include "tcp_server.hpp"
boost::int8_t mdl::tcp_server::init(boost::uint16_t __port_num) {
	this-> sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this-> sock == -1) return -1;

	this-> serveraddr.sin_family = AF_INET;
	this-> serveraddr.sin_addr.s_addr = htons(INADDR_ANY);
	this-> serveraddr.sin_port = htons(__port_num);
	this-> len = sizeof(this-> serveraddr);

	if (bind(this-> sock, (struct sockaddr*)&this-> serveraddr, this-> len) == -1) return -1;

	if (listen(this-> sock, 20) == -1) return -1;
	len = sizeof(this-> clientaddr);

	return 0;
}

boost::int8_t mdl::tcp_server::accept(int& __sock) {
	__sock = ::accept(this-> sock, (struct sockaddr*)&this-> clientaddr, &this-> len);
	if (__sock == -1) return -1;
}

boost::int8_t mdl::tcp_server::send(int __sock, boost::uint8_t *__buff, uint_t __buff_len) {
	return ::send(__sock, __buff, __buff_len, 0);
}

boost::int8_t mdl::tcp_server::recv(int __sock, boost::uint8_t *__buff, uint_t __buff_len) {
	return ::recv(__sock, __buff, __buff_len, 0);
}
/*
int main() {
	mdl::tcp_server tcp_server;
	tcp_server.init(21299);
}*/
