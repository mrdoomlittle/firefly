# ifndef __ffly__server__hpp
# define __ffly__server__hpp
# include "networking/tcp_server.hpp"
# include "networking/tcp_client.hpp"

# include "networking/udp_server.hpp"
# include "networking/udp_client.hpp"

# include <eint_t.hpp>
# include "types/uni_par_t.hpp"
# include <boost/thread.hpp>
# include "opencl_helper.hpp"
# include <serializer.hpp>
# include "types/client_info_t.hpp"
# include "types/player_info_t.hpp"
# include "types/worker_config_t.hpp"
# define FFLY_UNI_PAR_XLEN 2
# define FFLY_UNI_PAR_YLEN 2
# define FFLY_UNI_PAR_ZLEN 1
# include "ffly_config.hpp"
//# define FFLY_MAX_WORKERS 8
//# define FFLY_MIN_WORKERS 0
# include <signal.h>
# include <stdlib.h>
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
# include "keycodes.h"
# include <fstream>
# include <tuple>
# include "types/status.hpp"
# include <CL/cl.hpp>
# include "worker_manager.hpp"
# include "graphics/draw_pixmap.hpp"
namespace mdl { class ffly_server
{
	public:
	ffly_server(uint_t __uni_xlen, uint_t __uni_ylen, uint_t __uni_zlen)
	: uni_xlen(__uni_xlen), uni_ylen(__uni_ylen), uni_zlen(__uni_zlen),
	uni_particle_count((__uni_xlen * FFLY_UNI_PAR_XLEN) * (__uni_ylen * FFLY_UNI_PAR_YLEN) * (__uni_zlen * FFLY_UNI_PAR_ZLEN)), 
	worker_manager(FFLY_MIN_WORKERS, FFLY_MAX_WORKERS, (__uni_xlen * FFLY_UNI_PAR_XLEN), (__uni_ylen * FFLY_UNI_PAR_YLEN), (__uni_zlen * FFLY_UNI_PAR_ZLEN)) {}

	~ffly_server() {
		std::free(uni_particles);
	}

	boost::int8_t init();
	boost::int8_t begin();

	boost::uint8_t *create_pixmap(uint_t __xaxis_len, uint_t __yaxis_len, uint_t __offset) {
		boost::uint8_t *pixmap = static_cast<boost::uint8_t *>(malloc(((__xaxis_len * __yaxis_len) * __offset) * sizeof(boost::uint8_t)));
		return pixmap;
	}

	//void client_handler(int __sock, uint_t player_id);
	void player_handler(int __sock, uint_t player_id);

	uint_t add_player() {
		uint_t player_id = this-> player_index.size();
		this-> player_index.resize(this-> player_index.size() + 1);
		this-> connected_players ++;
		return player_id;
	}

	void del_player(uint_t __player_id) {
		firefly::types::player_info_t temp = this-> player_index[__player_id];
		if (this-> player_index.size() - 1 != 0) {
			this-> player_index[__player_id] = this-> player_index[this-> player_index.size() - 1];
		}
		this-> player_index.resize(this-> player_index.size() - 1);
		this-> connected_players --;
	}

	void draw_woker_pixmaps() {
		for (std::size_t o = 0; o != this-> worker_manager.worker_index.size(); o ++) {
			if (std::get<0>(this-> worker_manager.worker_index[o]) != nullptr) {
				firefly::types::worker_config_t& worker_config = *std::get<1>(this-> worker_manager.worker_index[o]);

				firefly::graphics::draw_pixmap(worker_config.chunk_xaxis, worker_config.chunk_yaxis, this-> uni_par_colours, 
				this-> uni_xlen * FFLY_UNI_PAR_XLEN, this-> uni_ylen * FFLY_UNI_PAR_YLEN, std::get<0>(this-> worker_manager.worker_index[o]), worker_config.chunk_xlen, worker_config.chunk_ylen, &this-> opencl);
			}
		}
	}

	//private:
	bool accepting_players = false;
	private:
	uint_t uni_dimensions = 3;
	firefly::worker_manager worker_manager;

	ublas::vector<firefly::types::player_info_t> player_index;

	uint_t connected_players = 0;

	bool shutdown_queued = false;
	firefly::opencl opencl;

	firefly::networking::tcp_server cl_tcp_stream, wk_tcp_stream;
	firefly::networking::udp_server cl_udp_stream, wk_udp_stream;

	uint_t const uni_particle_count = 0;

	firefly::types::uni_par_t * uni_particles = nullptr;
	boost::uint8_t *uni_par_colours = nullptr;

	uint_t const uni_xlen = 0, uni_ylen = 0, uni_zlen = 0;
};
}

# endif /*__ffly__server__hpp*/
