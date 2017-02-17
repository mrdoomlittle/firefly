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
# include "serial.hpp"
# include "types/player_info_t.hpp"
# define FFLY_UNI_PAR_XLEN 2
# define FFLY_UNI_PAR_YLEN 2
# define FFLY_UNI_PAR_XLEN 2
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
# include "keycodes.h"
# include <fstream>
# include <CL/cl.hpp>
namespace mdl { class ffly_server
{
	public:
	ffly_server(uint_t __uni_xlen, uint_t __uni_ylen, uint_t __uni_zlen)
	: uni_xlen(__uni_xlen), uni_ylen(__uni_ylen), uni_zlen(__uni_zlen), 
	uni_particle_count((__uni_xlen * FFLY_UNI_PAR_XLEN) * (__uni_ylen * FFLY_UNI_PAR_YLEN) * (__uni_zlen * FFLY_UNI_PAR_XLEN)) {}

	~ffly_server() {
		std::free(uni_particles);
	}

	boost::int8_t init();
	boost::int8_t begin();

	void uni_wmanager();
	void client_handler(int __sock, uint_t player_id);
	uint_t add_player() {
		uint_t player_id = this-> player_index.size();
		this-> player_index.resize(this-> player_index.size() + 1);
		return player_id;
	}

	void del_player(uint_t __player_id) {
		firefly::types::player_info_t temp = this-> player_index[__player_id];
		if (this-> player_index.size() - 1 != 0) {
			this-> player_index[__player_id] = this-> player_index[this-> player_index.size() - 1];
		}
		this-> player_index.resize(this-> player_index.size() - 1);
	}

	private:
	ublas::vector<firefly::types::player_info_t> player_index;

	cl::Context *cl_context;
	cl::Program *cl_program;
	cl::CommandQueue *cl_queue;
	cl::Buffer *uni_xlen_buff;
	cl::Buffer *cl_uni_buff;
	uint_t connected_players = 0;

	firefly::opencl opencl;

	firefly::networking::tcp_server cl_tcp_stream;
	firefly::networking::udp_server cl_udp_stream;
	uint_t const uni_particle_count = 0;

	firefly::types::uni_par_t * uni_particles = nullptr;
	boost::uint8_t *uni_par_colours = nullptr;

	uint_t const uni_xlen = 0, uni_ylen = 0, uni_zlen = 0;
};
}

# endif /*__ffly__server__hpp*/
