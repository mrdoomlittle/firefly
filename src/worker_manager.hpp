# ifndef __worker__manager__hpp
# define __worker__manager__hpp
# include <eint_t.hpp>
# include <boost/cstdint.hpp>
# include "networking/tcp_server.hpp"
# include "networking/udp_server.hpp"
# include "types/status.hpp"
# include <boost/thread.hpp>
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
# include <utility>
# include "types/worker_config_t.hpp"
# include <serializer.hpp>
# include "memory/alloc_pixmap.hpp"
# define FFLY_WORKER_CHUNK_XLEN 256
# define FFLY_WORKER_CHUNK_YLEN 256
# define FFLY_WORKER_CHUNK_ZLEN 1
namespace mdl {
namespace firefly {
class worker_manager {
	public:
	worker_manager(uint_t __min_workers, uint_t __max_workers)
	: min_workers(__min_workers), max_workers(__max_workers) {}

	boost::int8_t begin_listening() {
		if (this-> tcp_stream.init(21298) == FFLY_FAILURE) return FFLY_FAILURE;
		if (this-> udp_stream.init(10197) == FFLY_FAILURE) return FFLY_FAILURE;
		//this-> udp_stream.recv_ack();
		boost::thread(boost::bind(&worker_manager::listen, this));
	}

	void worker_handler(int __sock, uint_t __worker_id);

	void listen();

	~worker_manager() {
		for (std::size_t o = 0; o != this-> worker_index.size(); o ++)
			std::free(this-> worker_index[o].first);
	}

	uint_t add_worker() {
		uint_t worker_id = this-> worker_index.size();
		this-> worker_index.resize(this-> worker_index.size() + 1);

		this-> worker_index[worker_id].first = memory::alloc_pixmap(
			FFLY_WORKER_CHUNK_XLEN,
			FFLY_WORKER_CHUNK_YLEN,
			FFLY_WORKER_CHUNK_ZLEN
		);

		types::worker_config_t& worker_config = this-> worker_index[worker_id].second;

		worker_config.chunk_xlen = FFLY_WORKER_CHUNK_XLEN;
		worker_config.chunk_ylen = FFLY_WORKER_CHUNK_YLEN;
		worker_config.chunk_zlen = FFLY_WORKER_CHUNK_ZLEN;

		worker_config.chunk_xaxis = 0;
		worker_config.chunk_yaxis = 0;
		worker_config.chunk_zaxis = 0;

		this-> connected_workers ++;
		return worker_id;
	}

	void del_worker(uint_t __worker_id) {
		if (__worker_id == this-> worker_index.size() - 1) {
			std::free(this-> worker_index[__worker_id].first);
			this-> worker_index.resize(this-> worker_index.size() - 1);
		} else {
			uint_t end_worker_id = this-> worker_index.size() - 1;

			std::free(this-> worker_index[__worker_id].first);

			this-> worker_index[__worker_id].first = this-> worker_index[end_worker_id].first;
			this-> worker_index[__worker_id].second = this-> worker_index[end_worker_id].second;
			this-> worker_index.resize(this-> worker_index.size() - 1);
		}

		this-> connected_workers --;
	}

	bool insufficient() {
		return this-> connected_workers >= this-> min_workers? false : true;
	}

	ublas::vector<std::pair<boost::uint8_t *, types::worker_config_t>> worker_index;

	firefly::networking::tcp_server tcp_stream;
	firefly::networking::udp_server udp_stream;

	uint_t const min_workers, max_workers;
	uint_t connected_workers = 0;
};


}
}

# endif /*__worker__manager__hpp*/
