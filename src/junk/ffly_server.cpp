# include "ffly_server.hpp"
bool mdl::ffly_server::to_shutdown = false;

mdl::firefly::types::err_t mdl::ffly_server::init(uint_t __mn_players, uint_t __mx_players) {

}

mdl::firefly::types::err_t mdl::ffly_server::begin(boost::uint16_t __tcp_port_no, boost::uint16_t __udp_port_no) {
	if (this-> cl_tcp_server.init(__tcp_port_no) == -1) return FFLY_FAILURE;
	if (this-> cl_udp_server.init(__udp_port_no) == -1) return FFLY_FAILURE;
	this-> cl_tcp_server.listen();

	if (this-> worker_manager.begin_listen(21298, 10197) == FFLY_FAILURE)
		return FFLY_FAILURE;

	do {
		while(this-> worker_manager.insufficient()) {
			if (ffly_server::to_shutdown) break;
		}

		if (this-> player_manager.player_count() > this-> mx_players) continue;

		int sock;
		printf("waiting for player/s to connect to server.\n");
		if (this-> cl_tcp_server.accept(sock) == -1) {
			fprintf(stderr, "ffly_server: failed to accept.\n");
			return FFLY_FAILURE;
		}

		firefly::types::__id_t player_id;
		if (this-> player_manager.add_player(player_id) == FFLY_FAILURE)
			return FFLY_FAILURE;

		if (this-> player_manager.null_player_id(player_id))
			boost::thread(&firefly::player_handler, &this-> player_manager, nullptr, sock, player_id);

		printf("player has connected to server. there are now %d connected.\n\n", this-> player_manager.player_count());
	} while(!ffly_server::to_shutdown);
}

int main() {
	printf("yes.\n");
	mdl::firefly::types::uni_prop_t uni_prop = {
		.xaxis_len = 128,
		.yaxis_len = 128,
		.zaxis_len = 128
	};

	mdl::ffly_server ffly_server(uni_prop);
	ffly_server.init(0, 12);
	ffly_server.begin(21299, 10198);
}
