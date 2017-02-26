# include "uni_worker.hpp"

boost::int8_t mdl::firefly::uni_worker::begin(char const *__server_ipaddr) {
	mdl::firefly::networking::tcp_client tcp_stream;
	mdl::firefly::networking::udp_client udp_stream;

	types::worker_config_t worker_config = {0, 0, 0, 0, 0, 0};

	int sock;
	do {
		mdl::int_t sock_result = 0;
		if (!this-> server_connection) {
			if (tcp_stream.init(__server_ipaddr, 21298, sock) == -1) {
				this-> server_connection = false;
				fprintf(stderr, "failed to connect to server. attempt: %d\n", this-> connect_trys);
				goto skip;
			}

			if (udp_stream.init(__server_ipaddr, 10197) == -1) {
				this-> server_connection = false;
				fprintf(stderr, "failed to connect to server. attempt: %d\n", this-> connect_trys);
				goto skip;
			}

			//udp_stream.send_ack();


			if (tcp_stream.connect() == -1) {
				this-> server_connection = false;
				fprintf(stderr, "failed to connect to server. attempt: %d\n", this-> connect_trys);
				goto skip;
			}

			serializer serialize('\0');
			std::size_t cf_size = serialize.get_size(&worker_config);
			serialize.init(cf_size);
			serialize | 'w';

			tcp_stream.recv(serialize.get_serial(), cf_size);
			worker_config.achieve(serialize);
			serialize.reset();

			if (this-> uni_chunk != nullptr) {
				std::free(this-> uni_chunk);
			}

			this-> uni_chunk = memory::alloc_pixmap(worker_config.chunk_xlen, worker_config.chunk_ylen, worker_config.chunk_zlen);
			this-> uni_chunk_size = (worker_config.chunk_xlen * worker_config.chunk_ylen * worker_config.chunk_zlen) * 4;

			printf("chunk xlen: %d, chunk ylen: %d, chunk zlen: %d\n", worker_config.chunk_xlen, worker_config.chunk_ylen, worker_config.chunk_zlen);
			printf("chunk xaxis: %d, chunk yaxis: %d, chunk zaxis: %d\n", worker_config.chunk_xaxis, worker_config.chunk_yaxis, worker_config.chunk_zaxis);


			this-> server_connection = true;
		}

		printf("sending %d bytes.\n", this-> uni_chunk_size);
		sock_result = udp_stream.send(this-> uni_chunk, this-> uni_chunk_size);

		if (sock_result == -1 || sock_result == 0) {
			this-> server_connection = false;
			return -1;
		}

		skip:
		if (!this-> server_connection) {
			if (connect_trys == MAX_CONN_TRYS - 1) return -1;
			else connect_trys ++;
			usleep(DELAY_BT_TRYS);
			continue;
		} 
	} while(1);
}

int main() {
	mdl::firefly::uni_worker uni_worker;
	uni_worker.begin("192.168.0.100");
}
