# ifndef __uni__worker__hpp
# define __uni__worker__hpp
# include "defaults.hpp"
# include "types/worker_config_t.hpp"
# include "networking/tcp_client.hpp"
# include "networking/udp_client.hpp"
# include <serializer.hpp>
# include "memory/alloc_pixmap.hpp"
namespace mdl {
namespace firefly {
class uni_worker
{
	public:

	~uni_worker() {
		std::free(this-> uni_chunk);
	}

	boost::int8_t init();
	boost::int8_t begin(char const *__server_ipaddr);

	uint_t uni_chunk_size = 0;
	boost::uint8_t *uni_chunk = nullptr;
	uint_t connect_trys = 0;
	bool server_connection = false;
};
}
}

# endif /*__uni__worker__hpp*/
