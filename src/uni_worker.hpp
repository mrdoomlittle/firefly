# ifndef __uni__worker__hpp
# define __uni__worker__hpp
# include "defaults.hpp"
# include "networking/tcp_client.hpp"
# include "networking/udp_client.hpp"
namespace mdl {
namespace firefly {
class uni_worker
{
	public:

	boost::int8_t init();
	boost::int8_t begin(char const *__server_ipaddr);
	private:
	uint_t connect_trys = 0;
	bool server_connection = false;
};
}
}

# endif /*__uni__worker__hpp*/
