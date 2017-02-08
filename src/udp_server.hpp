# ifndef __udp__server__hpp
# define __udp__server__hpp
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstdio>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <math.h>
# include "/home/daniel-robson/Projects/eint_t/inc/eint_t.hpp"
# include <boost/cstdint.hpp>

# define PACKET_SIZE 54000

namespace mdl { class udp_server
{
	public:
	boost::uint8_t init(boost::uint16_t __port_num);
	void send(boost::uint8_t *__buff, uint_t __buff_len);
    void recv(boost::uint8_t *__buff, uint_t __buff_len);

	private:
	int len, sock;
	struct sockaddr_in server, client;
	socklen_t clientlen;

};
}

# endif /*__udp__server__hpp*/
