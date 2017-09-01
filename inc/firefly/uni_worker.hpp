# ifndef __uni__worker__hpp
# define __uni__worker__hpp
# include "types/err_t.h"
# include "system/errno.h"
# include "ffly_config.hpp"
# include "networking/tcp_client.hpp"
# include "networking/udp_client.hpp"
# include "types/worker_config_t.hpp"
# include <serializer.hpp>
# include <signal.h>
namespace mdl {
namespace firefly {
class uni_worker {
	public:
	types::err_t init();
	types::err_t begin();
	types::err_t de_init();
	types::err_t recv_config();

	types::err_t connect_ts(int& __sock, char const *__ip_addr, boost::uint16_t __tcp_port_no, boost::uint16_t __udp_port_no);

	static bool to_shutdown;
	bool connected_ts = false;
	boost::uint8_t connect_trys = 0;
	private:
	types::worker_config_t worker_config;
	networking::tcp_client tcp_client;
	networking::udp_client udp_client;
};
}
}

# endif /*__uni__worker__hpp*/
