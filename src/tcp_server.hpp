# ifndef __tcp__server__hpp
# define __tcp__server__hpp
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstdio>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include "../eint_t/inc/eint_t.hpp"
# include <boost/cstdint.hpp>
namespace mdl { class tcp_server
{
	public:
	boost::uint8_t init(boost::uint16_t __port_num);
	void send(int __sock, boost::uint8_t *__buff, uint_t __buff_len);
	void recv(int __sock, boost::uint8_t *__buff, uint_t __buff_len);

	template<typename __T>
	void send(int __sockm, __T __data) {
		std::size_t type_size = sizeof(__T);
		for (std::size_t o = 0; o != type_size; o ++) {
			boost::uint8_t data_to_send[1] = {0};
			if (o == 0) data_to_send[0] = __data & 0xFF;
			else
				data_to_send[0] = (__data >> (o * 8)) & 0xFF;

			this-> send(data_to_send, 1);
		}
	}

	template<typename __T>
	void recv(int __sock, __T& __data) {
		int type_size = sizeof(__T);
		for (std::size_t o = 0; o != type_size; o ++) {
			boost::uint8_t data_recv[1] = {0};
			recv(__sock, data_recv, 1);

			__data |= (__data & 0xFF) | (data_recv[0] << (o * 8));
		}
	}

	template<typename __T>
	void send(int __sock, __T *__buff, uint_t __buff_len) {
		for (std::size_t o = 0; o != __buff_len; o ++) send<__T>(__sock, __buff[o]);
	}

	template<typename __T>
	void send(int __sock, __T **__buff, uint_t __buff_xlen, uint_t __buff_ylen) {
		for (std::size_t o = 0; o != __buff_ylen; o ++) send<__T>(__sock, __buff[o], __buff_xlen);
	}

	template<typename __T>
	void recv(int __sock, __T *__buff, uint_t __buff_len) {
		for (std::size_t o = 0; o != __buff_len; o ++) recv<__T>(__sock, __buff[o]);
	}

	template<typename __T>
	void recv(int __sock, __T *__buff, uint_t __buff_xlen, uint_t __buff_ylen) {
		for (std::size_t o = 0; o != __buff_ylen; o ++) recv<__T>(__sock, __buff[o], __buff_xlen);
	}

	void accept(int& __sock);

	public:
	socklen_t len;
	int sock, k, tsock;
	struct sockaddr_in clientaddr, serveraddr;
} ;
}

# endif
