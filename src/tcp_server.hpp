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
	void write(boost::uint8_t *__buff, uint_t __buff_len);
	void read(boost::uint8_t *__buff, uint_t __buff_len);

	void read(int& __data) {
		int int_size = sizeof(int);
		for (std::size_t o = 0; o != int_size; o ++) {
			boost::uint8_t data_recv[1] = {0};
			read(data_recv, 1);

			//printf("read %d\n", data_recv[0]);
			__data |= (__data & 0xFF) | (data_recv[0] << (o * 8));
		}
	}

	public:
	socklen_t len;
	int sock, k, tsock;
	struct sockaddr_in client, server;
} ;
}

# endif
