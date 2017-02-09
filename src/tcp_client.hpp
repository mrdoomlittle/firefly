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
	boost::uint8_t init(char const * __ip_addr, boost::uint16_t __port_num);
	void write(boost::uint8_t *__buff, uint_t __buff_len);
	void read(boost::uint8_t *__buff, uint_t __buff_len);

	void write(int __data) {
		int int_size = sizeof(int);
		for (std::size_t o = 0; o != int_size; o ++) {
			boost::uint8_t data_to_send[1] = {0};
			if (o == 0)
				data_to_send[0] = __data & 0xFF;
			else
				data_to_send[0] = (__data >> (o * 8)) & 0xFF;

			//printf("writ %d\n", data_to_send[0]);
			this-> write(data_to_send, 1);
		}
	}

	private:
	int sock, k;
	struct sockaddr_in server;
} ;
}

# endif /*__tcp__client__hpp*/
