# include "worker_manager.hpp"
#include <errno.h>
#include <string.h>

void mdl::firefly::worker_manager::worker_handler(int __sock, uint_t *__worker_id) {
	uint_t worker_id = *__worker_id;

	printf("thread has started, worker id: %d\n", worker_id);

	types::worker_config_t& worker_config = std::get<1>(this-> worker_index[worker_id]);

	uint_t pixmap_size = (worker_config.chunk_xlen * worker_config.chunk_ylen * worker_config.chunk_zlen) * 4;

	boost::uint8_t *pixmap = std::get<0>(this-> worker_index[worker_id]);

	serializer serialize('\0');
	types::player_info_t dummy;
	std::size_t si_size = serialize.get_size(&dummy);
	std::size_t cf_size = serialize.get_size(&worker_config);
	serialize.init(cf_size);

	serialize | 'r';
	worker_config.achieve(serialize);
	serialize.reset();
	boost::int8_t *sock_state;

	sock_state = this-> tcp_stream.begin_poll(__sock, 1000000000);

	if (this-> tcp_stream.send(__sock, serialize.get_serial(), cf_size) == NET_FAULURE) {

	}

	// this player index will be sent to the worker
	ublas::vector<types::player_info_t> _player_index;

	uint_t in_range_players = 0;

	types::player_info_t pp = {0, 0, 0};

//	in_range_players ++;
//	_player_index.resize(in_range_players);
//	_player_index[0] = pp;

	do {
		if (*sock_state == SOCK_DEAD) {
			//printf("worker has disconnect i think.\n");
		}

		in_range_players = 0;
		std::size_t crr = 0;
		for (std::size_t o = 0; o != (*this-> player_index).size(); o ++) {
			types::player_info_t& player_info = (*this-> player_index)[o];

			if ((player_info.xaxis >= worker_config.chunk_xaxis && player_info.xaxis <= (worker_config.chunk_xaxis + worker_config.chunk_xlen))
			&& (player_info.yaxis >= worker_config.chunk_yaxis && player_info.yaxis <= (worker_config.chunk_yaxis + worker_config.chunk_ylen))) {
//			&& (player_info.zaxis > worker_config.chunk_zaxis && player_info.zaxis < (worker_config.chunk_zaxis + worker_config.chunk_zlen))) {
				in_range_players ++;
		//		printf("player has been found in chunk.\n");
				_player_index.resize(in_range_players);
				_player_index[crr] = player_info;
				crr ++;
			}
		}

		if (in_range_players == 0) continue;
		//printf("player has entered chunk.\n");

		boost::int8_t sock_result = 0;

		if ((sock_result = tcp_stream.send(__sock, in_range_players)) == NET_FAULURE) {
			fprintf(stderr, "error worker with id: %d has timedout or is offline, error code: %d, %s\n", worker_id, sock_result, strerror(errno));
			break;
		}

		for (std::size_t o = 0; o != in_range_players; o ++) {
			_player_index[o].achieve(serialize);
			serialize.reset();

			if ((sock_result = this-> tcp_stream.send(__sock, serialize.get_serial(), si_size)) == NET_FAULURE) {
				fprintf(stderr, "error worker with id: %d has timedout or is offline, error code: %d, %s\n", worker_id, sock_result, strerror(errno));
				break;
			}
		}

		if ((sock_result = this-> udp_stream.recv(pixmap, pixmap_size)) == NET_FAULURE) {
			fprintf(stderr, "error worker with id: %d has timedout or is offline, error code: %d, %s\n", worker_id, sock_result, strerror(errno));
			break;
		}

	} while(true);

	end:
	printf("worker with id: %d, thread has ended.\n\n", worker_id);

	this-> del_worker(__worker_id);
}

void mdl::firefly::worker_manager::listen() {
	if (this-> tcp_stream.listen() == -1) return;


	printf("worker/s needed: %d, worker/s connected: %d\n", FFLY_MIN_WORKERS, this-> connected_workers);
	do {
		int sock;
		printf("waiting for worker/s to connect the server.\n");

		if (this-> tcp_stream.accept(sock) == -1) {
			printf("failed to accept. %s\n", strerror(errno));

			continue;
		}

		uint_t *worker_id = this-> add_worker();
		boost::thread(boost::bind(&worker_manager::worker_handler, this, sock, worker_id));
		printf("worker has connected to the server.\n");

	} while(true);
}
