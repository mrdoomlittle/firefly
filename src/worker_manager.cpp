# include "worker_manager.hpp"
#include <errno.h>
#include <string.h>

void mdl::firefly::worker_manager::worker_handler(int __sock, uint_t __worker_id) {
	printf("worker with id: %d, thread has started.\n", __worker_id);
	types::worker_config_t& worker_config = this-> worker_index[__worker_id].second;

	uint_t pixmap_size = (worker_config.chunk_xlen * worker_config.chunk_ylen * worker_config.chunk_zlen) * 4;

	boost::uint8_t *pixmap = this-> worker_index[__worker_id].first;

	serializer serialize('\0');
	std::size_t cf_size = serialize.get_size(&worker_config);
	serialize.init(cf_size);

	serialize | 'r';
	worker_config.achieve(serialize);
	serialize.reset();

	if (this-> tcp_stream.send(__sock, serialize.get_serial(), cf_size) == NET_FAULURE) {
		goto end;
	}

	do {
		boost::int8_t sock_result = 0;
		printf("recving %d bytes.\n", pixmap_size);
		sock_result = this-> udp_stream.recv(pixmap, pixmap_size);
		if (sock_result == -1) {
			fprintf(stderr, "error worker with id: %d has timedout or is offline, error code: %d, %s\n", __worker_id, sock_result, strerror(errno));
			break;
		}

	} while(true);

	end:
	printf("worker with id: %d, thread has ended.\n\n", __worker_id);

	this-> del_worker(__worker_id);
}

void mdl::firefly::worker_manager::listen() {
	if (this-> tcp_stream.listen() == -1) return;

	do {
		int sock;
		printf("waiting for workers to connect.\n");

		if (this-> tcp_stream.accept(sock) == -1) {
			printf("failed to accept. %s\n", strerror(errno));

			continue;
		}

		uint_t worker_id = this-> add_worker();
		boost::thread(boost::bind(&worker_manager::worker_handler, this, sock, worker_id));
		printf("worker has connected to the server.\n");

	} while(true);
}
