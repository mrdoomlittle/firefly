# ifndef __tcp__client__hpp
# define __tcp__client__hpp
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
namespace mdl { class tcp_client
{
	public:
	boost::int8_t init(char const * __ip_addr, boost::uint16_t __portno, int& __sock);

	boost::int8_t send(boost::uint8_t *__buff, uint_t __buff_len);
	boost::int8_t recv(boost::uint8_t *__buff, uint_t __buff_len);

	template<typename __T>
	boost::int8_t send(__T __data) {
		std::size_t type_size = sizeof(__T);
		for (std::size_t o = 0; o != type_size; o ++) {
			boost::uint8_t data_to_send[1] = {0x0};
			if (o == 0) data_to_send[0] = __data & 0xFF;
			else
				data_to_send[0] = (__data >> (o * 8)) & 0xFF;

			boost::int8_t sock_state = this-> send(data_to_send, 1);
			if (sock_state == -1) return -1;
		}
		return 0;
	}

	template<typename __T>
	boost::int8_t recv(__T& __data) {
		std::size_t type_size = sizeof(__T);
		for (std::size_t o = 0; o != type_size; o ++) {
			boost::uint8_t data_recv[1] = {0x0};

			boost::int8_t sock_state = this-> recv(data_recv, 1);
			if (sock_state == -1) return -1;

			__data |= (__data & 0xFF) | (data_recv[0] << (o * 8));
		}
		return 0;
	}

	template<typename __T>
	boost::int8_t send(__T *__buff, uint_t __buff_len) {
		for (std::size_t o = 0; o != __buff_len; o ++) {
			boost::int8_t sock_state = this-> send<__T>(__buff[o]);
			if (sock_state == -1) return -1;
		}
		return 0;
	}

	template<typename __T>
	boost::int8_t send(__T **__buff, uint_t __buff_xlen, uint_t __buff_ylen) {
		for (std::size_t o = 0; o != __buff_ylen; o ++) {
			boost::int8_t sock_state = this-> send<__T>(__buff[o], __buff_xlen);
			if (sock_state == -1) return -1;
		}
		return 0;
	}

	template<typename __T>
	boost::int8_t recv(__T *__buff, uint_t __buff_len) {
		for (std::size_t o = 0; o != __buff_len; o ++) {
			boost::int8_t sock_state = this-> recv<__T>(__buff[o]);
			if (sock_state == -1) return -1;
		}
		return 0;
	}

	template<typename __T>
	boost::int8_t recv(__T *__buff, uint_t __buff_xlen, uint_t __buff_ylen) {
		for (std::size_t o = 0; o != __buff_ylen; o ++) {
			boost::int8_t sock_state = this-> recv<__T>(__buff[o], __buff_xlen);
			if (sock_state == -1) return -1;
		}
		return 0;
	}

	boost::int8_t connect();

	private:
	int sock;
	struct sockaddr_in serveraddr;
} ;
}

# endif /*__tcp__client__hpp*/
