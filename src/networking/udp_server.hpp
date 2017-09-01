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
# include <errno.h>
# include <math.h>
# include <mdlint.h>
# include <boost/cstdint.hpp>
# include "../pk_config.hpp"
# include "../defaults.hpp"
# include "../types/byte_t.h"
# include "../system/errno.h"
# include "../types/err_t.h"
static int dummy_sock = NULL;
namespace mdl {
namespace firefly {
namespace networking {
class udp_server
{
	public:
	types::err_t init(boost::uint16_t __port_no);
	types::err_t send(types::byte_t *__buff, uint_t __buff_len) {
		return this-> send(__buff, __buff_len, this-> clientaddr);
	}

	types::err_t recv(types::byte_t *__buff, uint_t __buff_len) {
		return this-> recv(__buff, __buff_len, this-> clientaddr);
	}

	types::err_t send_ack() {
		return this-> send_ack(this-> clientaddr);
	}

	types::err_t recv_ack() {
		return this-> recv_ack(this-> clientaddr);
	}

	types::err_t send(types::byte_t *__buff, uint_t __buff_len, struct sockaddr_in& __clientaddr);
    types::err_t recv(types::byte_t *__buff, uint_t __buff_len, struct sockaddr_in& __clientaddr);
	types::err_t send_ack(struct sockaddr_in& __clientaddr);
	types::err_t recv_ack(struct sockaddr_in& __clientaddr);

	private:
	int len, sock, wbuff_size = WBUFFER_SIZE, rbuff_size = RBUFFER_SIZE;
	struct sockaddr_in serveraddr, clientaddr;
	socklen_t clientlen;

};
}
}
}

# endif /*__udp__server__hpp*/
