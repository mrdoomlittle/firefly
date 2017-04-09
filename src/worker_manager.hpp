# ifndef __worker__manager__hpp
# define __worker__manager__hpp
# include <errno.h>
# include <eint_t.hpp>
# include "types/id_t.hpp"
# include "system/errno.h"
# include "types/err_t.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_init.h"
# include "types/worker_config_t.hpp"
# include "types/pixmap_t.h"
# include <tuple>
# include <set>
# include <boost/cstdint.hpp>
# include "uni_manager.hpp"
# include "networking/tcp_server.hpp"
# include "networking/udp_server.hpp"
# include <boost/numeric/ublas/vector.hpp>
# include "memory/alloc_pixmap.hpp"
# include "system/config.hpp"
# include <atomic>
# include "system/th_control.hpp"
namespace ublas = boost::numeric::ublas;
namespace mdl {
namespace firefly {
class worker_manager;
extern types::err_t worker_handler(worker_manager *__worker_manager, int __sock, struct sockaddr_in __client_addr, types::id_t __worker_id);
}
}

namespace mdl {
namespace firefly {
class worker_manager {
	friend types::err_t worker_handler(worker_manager *__worker_manager, int __sock, struct sockaddr_in __client_addr, types::id_t __worker_id);
	public:
	worker_manager(uint_t __mn_workers, uint_t __mx_workers, uni_manager *__uni_manager)
	: mn_workers(__mn_workers), mx_workers(__mx_workers), _uni_manager(__uni_manager) {}

	types::err_t add_worker(types::id_t& __worker_id);
	types::err_t rm_worker(types::id_t __worker_id);

	types::err_t begin_listen(boost::uint16_t __tcp_port_no, boost::uint16_t __udp_port_no) {
		if (this-> tcp_server.init(__tcp_port_no) == FFLY_FAILURE)
			return FFLY_FAILURE;
		if (this-> udp_server.init(__udp_port_no) == -1) return FFLY_FAILURE;

		boost::thread(boost::bind(&worker_manager::listen, this));
		return FFLY_SUCCESS;
	}

	types::err_t de_init();
	types::err_t listen();

	uint_t& chunk_xlen() {return this-> chunk_xaxis_len;}
	uint_t& chunk_ylen() {return this-> chunk_yaxis_len;}
	uint_t& chunk_zlen() {return this-> chunk_zaxis_len;}

	bool insufficient() {
		return this-> connected_workers >= this-> mn_workers? false : true;
	}

	enum {
		MSG_NULL,
		INX_SHIFT
	};

	static bool to_shutdown;
	system::th_control th_control = {
		.th_pause = false,
		.th_waiting = false,
		.th_id = 0,
		.th_msg = MSG_NULL,
		.th_reply = TH_NULL
	};

	private:
	uint_t nxt_xaxis = 0, nxt_yaxis = 0, nxt_zaxis = 0;
	uint_t xchunk_c = 0, ychunk_c = 0, zchunk_c = 0;
	uint_t nxt_xchunk_id = 0, nxt_ychunk_id = 0, nxt_zchunk_id = 0;

	ublas::vector<std::tuple<types::pixmap_t, types::worker_config_t *, types::id_t>> worker_indx;

	uint_t chunk_xaxis_len = system::config.wk_chunk_xlen,
		chunk_yaxis_len = system::config.wk_chunk_ylen,
		chunk_zaxis_len = system::config.wk_chunk_zlen;

	networking::tcp_server tcp_server;
    networking::udp_server udp_server;

	uni_manager const *_uni_manager;

	uint_t const mn_workers, mx_workers;
	std::atomic<uint_t> connected_workers;
};
}
}

# endif /*__worker__manager__hpp*/
