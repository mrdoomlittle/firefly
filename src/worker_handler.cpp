# include "types/err_t.h"
# include "types/id_t.hpp"
# include "worker_manager.hpp"
# include <cstdio>
# include <serializer.hpp>
namespace mdl {
namespace firefly {
types::err_t worker_handler(worker_manager *__worker_manager, int __sock, struct sockaddr_in __client_addr, types::id_t __worker_id) {
	types::worker_config_t *worker_config = std::get<1>(__worker_manager-> worker_indx[*__worker_id]);
	uint_t worker_id = *__worker_id;

	auto send_config = [__worker_manager, __sock, worker_config]() {
		serializer serialize('\0');
		std::size_t cf_size = serialize.get_size(worker_config);

		serialize.init(cf_size);
		serialize | 'r';

		worker_config-> achieve(serialize);
		__worker_manager-> tcp_server.send(__sock, serialize.get_serial(), cf_size);
	};

	types::err_t *sock_state = __worker_manager-> tcp_server.begin_poll(__sock, 1000000000);

	printf("thread for worker %d has started.\n", worker_id);
	do {
		if (*sock_state == SOCK_DEAD) {
			fprintf(stderr, "worker_handler: worker has disconnected.\n");
			break;
		}

		if (__worker_manager-> th_control.th_pause && __worker_manager-> th_control.th_id == worker_id) {
			__worker_manager-> th_control.th_waiting = true;

			while(__worker_manager-> th_control.th_pause) {
				if (__worker_manager-> th_control.th_msg == worker_manager::INX_SHIFT) {
					// udate id
					uint_t old = worker_id;
					worker_id = *__worker_id;
					printf("updated worker id from %d to %d\n", old, worker_id);
					__worker_manager-> th_control.th_reply = TH_SUCCESS;
					break;
				}
			}

			while(__worker_manager-> th_control.th_pause) {}
		}
	} while(!worker_manager::to_shutdown);

	__worker_manager-> rm_worker(__worker_id);

	printf("thread for worker %d has ended.\n", worker_id);
}
}
}
