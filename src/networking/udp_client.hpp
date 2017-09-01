# ifndef __udp__client__hpp
# define __udp__client__hpp
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstdio>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <math.h>
# include <errno.h>
# include <mdlint.h>
# include <boost/cstdint.hpp>
# include "../pk_config.hpp"
# include "../defaults.hpp"
# include "../types/byte_t.h"
# include "../system/errno.h"
# include "../types/err_t.h"
namespace mdl {
namespace firefly {
namespace networking {
class udp_client
{
	public:
	types::err_t init(char const * __ip_addr, boost::uint16_t __port_no);
	types::err_t send(types::byte_t *__buff, uint_t __buff_len);
	types::err_t recv(types::byte_t *__buff, uint_t __buff_len);
	types::err_t send_ack();
	types::err_t recv_ack();

	private:
	int sock, wbuff_size = WBUFFER_SIZE, rbuff_size = RBUFFER_SIZE;
	socklen_t sock_len;
	struct sockaddr_in serveraddr, clientaddr;
};
}
}
}

# endif /*__udp__client__hpp*/
