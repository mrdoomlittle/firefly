# include "uni_worker.hpp"

mdl::uni_worker::uni_worker(char const * __wmanager_ip) : wmanager_ip(__wmanager_ip) {

	this-> _tcp_client.init(__wmanager_ip, 21299);
	this-> _udp_client.init(__wmanager_ip, 10198);

	this-> get_worker_info();

	this-> particle_amount = (this-> worker_info.chunk_xlen * this-> worker_info.chunk_ylen);
	this-> uni_chunk_size = (this-> particle_amount * 4);
	this-> uni_chunk = static_cast<boost::uint8_t *>(malloc(this-> uni_chunk_size));
	bzero(this-> uni_chunk, this-> uni_chunk_size);
}

boost::uint8_t mdl::uni_worker::begin() {
	do  {
		this-> recv_player_coords();

	} while(true);
}

int main() {
	mdl::uni_worker uni_worker("192.168.0.100");

}
