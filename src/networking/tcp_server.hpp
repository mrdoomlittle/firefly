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
# include <sys/time.h>
# include <eint_t.hpp>
# include <boost/cstdint.hpp>
namespace mdl { 
namespace firefly {
namespace networking {
class tcp_server
{
	public:
	boost::int8_t init(boost::uint16_t __port_num);
	boost::int8_t send(int __sock, boost::uint8_t *__buff, uint_t __buff_len);
	boost::int8_t recv(int __sock, boost::uint8_t *__buff, uint_t __buff_len);

	boost::int8_t listen();
	void close(int __sock) {
		::close(__sock);
	}

	template<typename __T>
	boost::int8_t send(int __sock, __T __data) {
		std::size_t type_size = sizeof(__T);
		for (std::size_t o = 0; o != type_size; o ++) {
			boost::uint8_t data_to_send[1] = {0x0};
			if (o == 0) data_to_send[0] = __data & 0xFF;
			else
				data_to_send[0] = (__data >> (o * 8)) & 0xFF;

			boost::int8_t sock_state = this-> send(__sock, data_to_send, 1);
			if (sock_state == -1 || sock_state == 0) return sock_state;
		}
	}

	template<typename __T>
	boost::int8_t recv(int __sock, __T& __data) {
		int type_size = sizeof(__T);
		for (std::size_t o = 0; o != type_size; o ++) {
			boost::uint8_t data_recv[1] = {0x0};
			boost::int8_t sock_state = this-> recv(__sock, data_recv, 1);
			if (sock_state == -1 || sock_state == 0) return sock_state;

			__data |= (__data & 0xFF) | (data_recv[0] << (o * 8));
		}
	}

	template<typename __T>
	boost::int8_t send(int __sock, __T *__buff, uint_t __buff_len) {
		for (std::size_t o = 0; o != __buff_len; o ++) {
			boost::int8_t sock_state = this-> send<__T>(__sock, __buff[o]);
			if (sock_state == -1) return -1;
		}
	}

	template<typename __T>
	boost::int8_t send(int __sock, __T **__buff, uint_t __buff_xlen, uint_t __buff_ylen) {
		for (std::size_t o = 0; o != __buff_ylen; o ++) {
			boost::int8_t sock_state = this-> send<__T>(__sock, __buff[o], __buff_xlen);
			if (sock_state == -1) return -1;
		}
	}

	template<typename __T>
	boost::int8_t recv(int __sock, __T *__buff, uint_t __buff_len) {
		for (std::size_t o = 0; o != __buff_len; o ++) {
			boost::int8_t sock_state = this-> recv<__T>(__sock, __buff[o]);
			if (sock_state == -1) return -1;
		}
	}

	template<typename __T>
	boost::int8_t recv(int __sock, __T *__buff, uint_t __buff_xlen, uint_t __buff_ylen) {
		for (std::size_t o = 0; o != __buff_ylen; o ++) {
			boost::int8_t sock_state = this-> recv<__T>(__sock, __buff[o], __buff_xlen);
			if (sock_state == -1) return -1;
		}
	}

	boost::int8_t accept(int& __sock);

	public:
	socklen_t len;
	int sock, k, tsock;
	struct sockaddr_in clientaddr, serveraddr;
} ;
}
}
}

# endif
