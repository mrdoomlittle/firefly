# ifndef __ffly__server__hpp
# define __ffly__server__hpp

# include "networking/tcp_server.hpp"
# include "networking/tcp_client.hpp"

# include "networking/udp_server.hpp"
# include "networking/udp_client.hpp"
# include <mdlint.h>
# ifdef __USING_OPENCL
#	include "opencl_helper.hpp"
#	include <CL/cl.hpp>
# endif

# include "types/uni_prop_t.hpp"
# include "uni_manager.hpp"
# include "types/err_t.h"
# include "worker_manager.hpp"
# include "player_manager.hpp"
# include <cstdio>
namespace mdl {
namespace firefly {
extern types::err_t player_handler(player_manager *__player_manager, void *__this, int __sock, types::__id_t __player_id);
}
}
//extern mdl::firefly::types::err_t mdl::firefly::player_handler(player_manager *__player_manager, void *__this, int __sock, types::__id_t __player_id);
namespace mdl { class ffly_server {
	public:
	ffly_server(firefly::types::uni_prop_t __uni_prop): uni_prop(__uni_prop),
	uni_manager(__uni_prop.xaxis_len, __uni_prop.yaxis_len, __uni_prop.zaxis_len),
	worker_manager(0, 8, &uni_manager) {}

	static bool to_shutdown;

	firefly::types::err_t init(uint_t __mn_players, uint_t __mx_players);
	firefly::types::err_t begin(boost::uint16_t __tcp_port_no, boost::uint16_t __udp_port_no);

	protected:
	uint_t mn_players, mx_players;
	bool accepting_players;
	private:
	firefly::worker_manager worker_manager;
	firefly::player_manager player_manager;
# ifdef __USING_OPENCL
	firefly::opencl opencl_helper;
# endif
	firefly::networking::tcp_server cl_tcp_server, wk_tcp_server;
	firefly::networking::udp_server cl_udp_server, wk_udp_server;

	firefly::uni_manager uni_manager;

	firefly::types::uni_prop_t const uni_prop;
};
}

# endif /*__ffly__server__hpp*/
