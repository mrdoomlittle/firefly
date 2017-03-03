# ifndef __uni__worker__hpp
# define __uni__worker__hpp
# include "defaults.hpp"
# include "types/worker_config_t.hpp"
# include "networking/tcp_client.hpp"
# include "networking/udp_client.hpp"
# include <serializer.hpp>
# include <utility>
# include <signal.h>
# include <stdlib.h>
# include <boost/thread.hpp>
# include "memory/alloc_pixmap.hpp"
# include "types/player_info_t.hpp"
# include <boost/numeric/ublas/vector.hpp>
# include "memory/mem_free.h"
namespace ublas = boost::numeric::ublas;
namespace mdl {
namespace firefly {
class uni_worker
{
	public:

	void clean_up() {
		if (uni_chunk != nullptr) {
			memory::mem_free(uni_chunk);
			uni_chunk = nullptr;
		}
	}

	~uni_worker() {
		if (uni_chunk != nullptr) memory::mem_free(this-> uni_chunk);
	}

	void handle_player(uint_t __player_id);

	bool can_handle = false;
	bool handle_done = false;

	bool is_running = false;
	bool shutdown_queued = false;

	boost::int8_t init();
	boost::int8_t begin(char const *__server_ipaddr);

	ublas::vector<std::pair<types::player_info_t, bool>> player_index;
	types::worker_config_t *_worker_config;

	uint_t uni_chunk_size = 0;
	boost::uint8_t *uni_chunk = nullptr;
	uint_t connect_trys = 0;
	bool server_connection = false;
};
}
}

# endif /*__uni__worker__hpp*/
