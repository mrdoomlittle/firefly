# ifndef __uni__worker__hpp
# define __uni__worker__hpp
# include "tcp_client.hpp"
# include "udp_client.hpp"
# include <boost/cstdint.hpp>
# include "worker_info_t.hpp"
# include "../eint_t/inc/eint_t.hpp"
namespace mdl { class uni_worker {
	public:
	uni_worker(char const * __wmanager_ip);

	worker_info_t get_worker_info() {
		worker_info_t worker_info;
		serial archiver('\0');

		std::size_t size = archiver.get_size(&worker_info);
		archiver | 'w';
		archiver.init(size);

		this-> _tcp_client.recv(archiver.get_data(), size);
		worker_info.arc(archiver);
	}

	boost::uint8_t begin();

	void recv_player_coords() {
		this-> _tcp_client.recv(player_coords, 2);
	}

	private:
	boost::uint32_t player_coords[2] = {0x0};
	uint_t particle_amount = 0;
	uint_t uni_chunk_size = 0;
	boost::uint8_t * uni_chunk = nullptr;
	worker_info_t worker_info;
	tcp_client _tcp_client;
	udp_client _udp_client;
	char const * wmanager_ip = nullptr;
} ;
}


# endif /*__uni__worker__hpp*/
