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
# include <boost/thread.hpp>
# include <boost/cstdint.hpp>
# include "conn_status.h"
# include <chrono>
# include <atomic>
# include <poll.h>
# include <set>
# include <errno.h>
# include "../types/byte_t.h"
# include "../system/errno.h"
# include "../types/err_t.h"
namespace mdl {
namespace firefly {
namespace networking {
class tcp_server
{
	public:
	types::err_t init(boost::uint16_t __port_no);
	types::err_t de_init();
	types::err_t send(int __sock, types::byte_t *__buff, uint_t __buff_len);
	types::err_t recv(int __sock, types::byte_t *__buff, uint_t __buff_len);

	char *cl_ip_addr(int __sock) {
		struct sockaddr_in *s = &this-> client_addr;
		socklen_t len = sizeof(this-> client_addr);

		getpeername(__sock, (struct sockaddr*)&this-> client_addr, &len);

		static char ip_addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &s-> sin_addr, ip_addr, sizeof(ip_addr));
		return ip_addr;
	}

	static bool to_shutdown;

	void poll_handler(int __sock, boost::int8_t *__sock_state, uint_t const __delay_ns);
	boost::int8_t* begin_poll(int __sock, uint_t const __delay_ns);

	std::atomic<uint_t> poll_th_c;
	std::atomic<bool> can_poll;
	std::atomic<bool> poll_lock;
	bool polling_enabled = false;
	std::set<int> poll_sock;

	types::err_t listen();
	void close(int __sock) {
		::close(__sock);
	}

	template<typename __T>
	types::err_t send(int __sock, __T __data) {
		std::size_t type_size = sizeof(__T);
		for (std::size_t o = 0; o != type_size; o ++) {
			types::byte_t data_to_send[1] = {0x0};
			if (o == 0) data_to_send[0] = __data & 0xFF;
			else
				data_to_send[0] = (__data >> (o * 8)) & 0xFF;

			boost::int8_t sock_result = this-> send(__sock, data_to_send, 1);
			if (sock_result == FFLY_FAILURE) return FFLY_FAILURE;
		}
		return FFLY_FAILURE;
	}

	template<typename __T>
	types::err_t recv(int __sock, __T& __data) {
		int type_size = sizeof(__T);
		for (std::size_t o = 0; o != type_size; o ++) {
			types::byte_t data_recv[1] = {0x0};
			boost::int8_t sock_result = this-> recv(__sock, data_recv, 1);
			if (sock_result == FFLY_FAILURE) return FFLY_FAILURE;

			__data |= (__data & 0xFF) | (data_recv[0] << (o * 8));
		}
		return FFLY_SUCCESS;
	}

	template<typename __T>
	types::err_t send(int __sock, __T *__buff, uint_t __buff_len) {
		for (std::size_t o = 0; o != __buff_len; o ++) {
			boost::int8_t sock_result = this-> send<__T>(__sock, __buff[o]);
			if (sock_result == FFLY_FAILURE) return FFLY_FAILURE;
		}
		return FFLY_SUCCESS;
	}

	template<typename __T>
	types::err_t send(int __sock, __T **__buff, uint_t __buff_xlen, uint_t __buff_ylen) {
		for (std::size_t o = 0; o != __buff_ylen; o ++) {
			boost::int8_t sock_result = this-> send<__T>(__sock, __buff[o], __buff_xlen);
			if (sock_result == FFLY_FAILURE) return FFLY_FAILURE;
		}
		return FFLY_SUCCESS;
	}

	template<typename __T>
	types::err_t recv(int __sock, __T *__buff, uint_t __buff_len) {
		for (std::size_t o = 0; o != __buff_len; o ++) {
			boost::int8_t sock_result = this-> recv<__T>(__sock, __buff[o]);
			if (sock_result == FFLY_FAILURE) return FFLY_FAILURE;
		}
		return FFLY_SUCCESS;
	}

	template<typename __T>
	types::err_t recv(int __sock, __T *__buff, uint_t __buff_xlen, uint_t __buff_ylen) {
		for (std::size_t o = 0; o != __buff_ylen; o ++) {
			types::err_t sock_result = this-> recv<__T>(__sock, __buff[o], __buff_xlen);
			if (sock_result == FFLY_FAILURE) return FFLY_FAILURE;
		}
		return FFLY_SUCCESS;
	}

	boost::int8_t accept(int& __sock);

	public:
	socklen_t len;
	int sock, k, tsock;
	struct sockaddr_in client_addr, server_addr;
} ;
}
}
}

# endif
