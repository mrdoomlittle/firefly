# ifndef __worker__manager__hpp
# define __worker__manager__hpp
# include <mdlint.h>
# include <boost/cstdint.hpp>
# include "networking/tcp_server.hpp"
# include "networking/udp_server.hpp"
# include "types/status.hpp"
# include <boost/thread.hpp>
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
# include "ffly_config.hpp"
# include <utility>
# include "types/worker_config_t.hpp"
# include <serializer.hpp>
# include "types/player_info_t.hpp"
# include "memory/alloc_pixmap.hpp"
# define FFLY_WORKER_CHUNK_XLEN 256
# define FFLY_WORKER_CHUNK_YLEN 256
# define FFLY_WORKER_CHUNK_ZLEN 1
# include "networking/conn_status.h"
# include <tuple>
# include "memory/mem_free.h"
# include "uni_manager.hpp"
# include "system/errno.h"
# include <errno>
# include "types/id_t.hpp"
namespace mdl {
namespace firefly {
class worker_manager {
	public:
	worker_manager(uint_t __mn_workers, uint_t __mx_workers, uni_manager *__uni_manager)
	: mn_workers(__mn_workers), mx_workers(__mx_workers), _uni_manager(__uni_manager) {}
/*
	worker_manager(uint_t __mn_workers, uint_t __mx_workers, uint_t const __uni_xlen, uint_t const __uni_ylen, uint_t const __uni_zlen)
	: min_workers(__min_workers), max_workers(__max_workers), uni_xlen(__uni_xlen), uni_ylen(__uni_ylen), uni_zlen(__uni_zlen) {}
*/
	boost::int8_t begin_listening() {
		if (this-> tcp_stream.init(21298) == FFLY_FAILURE) return FFLY_FAILURE;
		if (this-> udp_stream.init(10197) == FFLY_FAILURE) return FFLY_FAILURE;

		boost::thread(boost::bind(&worker_manager::listen, this));
	}

	boost::int8_t init() {
		if (!this-> uni_xlen % FFLY_WORKER_CHUNK_XLEN) return FFLY_FAILURE;
		if (!this-> uni_ylen % FFLY_WORKER_CHUNK_YLEN) return FFLY_FAILURE;
		if (!this-> uni_zlen % FFLY_WORKER_CHUNK_ZLEN) return FFLY_FAILURE;
		this-> xchunk_count = this-> uni_xlen / FFLY_WORKER_CHUNK_XLEN;
		this-> ychunk_count = this-> uni_ylen / FFLY_WORKER_CHUNK_YLEN;
		this-> zchunk_count = this-> uni_zlen / FFLY_WORKER_CHUNK_ZLEN;
	}

	void worker_handler(int __sock, struct sockaddr_in __usock, uint_t *__worker_id);

	void listen();

	~worker_manager() {
		for (std::size_t o = 0; o != this-> worker_index.size(); o ++) {
			std::free(std::get<0>(this-> worker_index[o]));
			std::free(std::get<2>(this-> worker_index[o]));
		}
	}


	types::err_t add_worker(types::__id_t& __worker_id);
	types::err_t rm_worker(types::__id_t __worker_id);

	/*
			NOTE!!!!!!!
		for some resion the memory address to the worker_config for the worker_index is changing, i've changed it 
		so its gets allocated dynamically and it seem's to have fixed it.
	*/
/*
	uint_t* add_worker() {
		uint_t *_worker_id = static_cast<uint_t *>(malloc(sizeof(uint_t)));
		*_worker_id = 0;

		uint_t& worker_id = *_worker_id;
		worker_id = this-> worker_index.size();

		this-> worker_index.resize(this-> worker_index.size() + 1);

		std::get<0>(this-> worker_index[worker_id]) = memory::alloc_pixmap(
			FFLY_WORKER_CHUNK_XLEN,
			FFLY_WORKER_CHUNK_YLEN,
			FFLY_WORKER_CHUNK_ZLEN
		);

		memset(std::get<0>(this-> worker_index[worker_id]), 244, (FFLY_WORKER_CHUNK_XLEN * FFLY_WORKER_CHUNK_YLEN * FFLY_WORKER_CHUNK_ZLEN) * 4);

		types::worker_config_t& worker_config = *(std::get<1>(this-> worker_index[worker_id]) = new types::worker_config_t);

		worker_config.chunk_xlen = FFLY_WORKER_CHUNK_XLEN;
		worker_config.chunk_ylen = FFLY_WORKER_CHUNK_YLEN;
		worker_config.chunk_zlen = FFLY_WORKER_CHUNK_ZLEN;

		worker_config.chunk_xaxis = this-> next_xaxis;
		worker_config.chunk_yaxis = this-> next_yaxis;
		worker_config.chunk_zaxis = this-> next_zaxis;

		this-> connected_workers ++;

		std::get<2>(this-> worker_index[worker_id]) = _worker_id;

		if (this-> n_xchunk_id == this-> xchunk_count) {
			this-> n_xchunk_id = 0;
			this-> next_xaxis = 0;
			if (this-> n_ychunk_id == this-> ychunk_count) {
				this-> n_ychunk_id = 0;
				this-> next_yaxis = 0;
				if (this-> n_zchunk_id == this-> zchunk_count) {
				} else {
					this-> n_zchunk_id ++;
					this-> next_zaxis += FFLY_WORKER_CHUNK_ZLEN;
				}
			} else {
				this-> n_ychunk_id ++;
				this-> next_yaxis += FFLY_WORKER_CHUNK_YLEN;
			}
		} else {
			this-> n_xchunk_id ++;
			this-> next_xaxis += FFLY_WORKER_CHUNK_XLEN;
		}

		return _worker_id;
	}

	void del_worker(uint_t *__worker_id) {
		uint_t& worker_id = *__worker_id;

		if (worker_id == this-> worker_index.size() - 1) {
			memory::mem_free(std::get<0>(this-> worker_index[worker_id]));
			std::free(std::get<2>(this-> worker_index[worker_id]));
			std::free(std::get<1>(this-> worker_index[worker_id]));
			this-> worker_index.resize(this-> worker_index.size() - 1);
		} else {
			uint_t end_worker_id = this-> worker_index.size() - 1;

			std::free(std::get<0>(this-> worker_index[worker_id]));
			std::free(std::get<2>(this-> worker_index[worker_id]));

			std::get<0>(this-> worker_index[worker_id]) = std::get<0>(this-> worker_index[end_worker_id]);
			std::get<1>(this-> worker_index[worker_id]) = std::get<1>(this-> worker_index[end_worker_id]);
			std::get<2>(this-> worker_index[worker_id]) = std::get<2>(this-> worker_index[end_worker_id]);

			this-> worker_index.resize(this-> worker_index.size() - 1);
		}

		this-> connected_workers --;
	}
*/
	bool insufficient() {
		return this-> connected_workers >= this-> min_workers? false : true;
	}

	ublas::vector<std::tuple<boost::uint8_t *, types::worker_config_t *, uint_t *>> worker_index;
	uint_t const uni_xlen = 0, uni_ylen = 0, uni_zlen = 0;

	uint_t xchunk_count = 0, ychunk_count = 0, zchunk_count = 0;
	uint_t next_xaxis = 0, next_yaxis = 0, next_zaxis = 0;
	uint_t n_xchunk_id = 0, n_ychunk_id = 0, n_zchunk_id = 0;

	networking::tcp_server tcp_stream;
	networking::udp_server udp_stream;

	uni_manager *_uni_manager = nullptr;

	uint_t const mn_workers, mx_workers;
	uint_t connected_workers = 0;


	ublas::vector<types::player_info_t> *player_index;
};


}
}

# endif /*__worker__manager__hpp*/
