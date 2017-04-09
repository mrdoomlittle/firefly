# include <cstdio>
# include "uni_worker.hpp"
bool mdl::firefly::uni_worker::to_shutdown = false;

void ctrl_c(int sig) {
	mdl::firefly::uni_worker::to_shutdown = true;
}

mdl::firefly::types::err_t mdl::firefly::uni_worker::init() {
	struct sigaction sig_handler;

	sig_handler.sa_handler = ctrl_c;
	sigemptyset(&sig_handler.sa_mask);
	sig_handler.sa_flags = 0;

	sigaction(SIGINT, &sig_handler, NULL);
}

mdl::firefly::types::err_t mdl::firefly::uni_worker::de_init() {
	this-> tcp_client.de_init();
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::uni_worker::connect_ts(int& __sock, char const *__ip_addr, boost::uint16_t __tcp_port_no, boost::uint16_t __udp_port_no) {
	for (; this-> connect_trys != FFLY_MAX_CONN_TRYS; this-> connect_trys++) {
		if (this-> connect_trys != 0) {
			fprintf(stderr, "uni_worker: failed to connected to server, attempt: %d out of %d\n", this-> connect_trys, FFLY_MAX_CONN_TRYS);
			usleep(FFLY_DELAY_BT_TRYS);
		}

		this-> connected_ts = true;
		if (this-> tcp_client.init(__ip_addr, __tcp_port_no, __sock) == FFLY_FAILURE) {
			this-> connected_ts = false;
			fprintf(stderr, "uni_worker: failed to init tcp part, errno: %d\n", errno);
			continue;
		}

		if (this-> tcp_client.connect() == FFLY_FAILURE) {
			this-> connected_ts = false;
			fprintf(stderr, "uni_worker: failed to extablish connection to server, errno %d\n", errno);
			continue;
		}

		if (this-> udp_client.init(__ip_addr, __udp_port_no) == FFLY_FAILURE) {
			this-> connected_ts = false;
			fprintf(stderr, "uni_worker: failed to extablish connection to server, errno %d\n", errno);
			continue;
		}

		if (this-> connected_ts) {
			this-> connect_trys = 0;
			return FFLY_SUCCESS;
		}
	}

	return FFLY_FAILURE;
}

mdl::firefly::types::err_t mdl::firefly::uni_worker::recv_config() {
	serializer serialize('\0');
	std::size_t cf_size = serialize.get_size(&this-> worker_config);

	serialize.init(cf_size);
	serialize | 'w';

	this-> tcp_client.recv(serialize.get_serial(), cf_size);
	this-> worker_config.achieve(serialize);
	serialize.reset();
}

mdl::firefly::types::err_t mdl::firefly::uni_worker::begin() {
	int sock;
	if (this-> connect_ts(sock, "192.168.0.100", 21298, 10197) == FFLY_FAILURE) {
		return FFLY_FAILURE;
	}

	this-> udp_client.send_ack();
	this-> udp_client.recv_ack();

	tcp_client.begin_poll(1000000000);

	this-> recv_config();

	printf("worker now active.\n");
	do {



	} while(!uni_worker::to_shutdown);
	this-> de_init();

	return FFLY_SUCCESS;
}

int main() {
	mdl::firefly::uni_worker uni_worker;

	uni_worker.init();
	uni_worker.begin();
}
