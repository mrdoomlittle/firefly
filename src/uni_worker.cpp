# include "uni_worker.hpp"

boost::int8_t mdl::firefly::uni_worker::begin(char const *__server_ipaddr) {
	mdl::firefly::networking::tcp_client tcp_stream;
	mdl::firefly::networking::udp_client udp_stream;

	int sock;
	do {
		if (!this-> server_connection) {
			if (tcp_stream.init(__server_ipaddr, 21298, sock) == -1) {
				this-> server_connection = false;
				fprintf(stderr, "failed to connect to server. attempt: %d\n", this-> connect_trys);
				goto skip;
			}

			if (tcp_stream.connect() == -1) {
				this-> server_connection = false;
				fprintf(stderr, "failed to connect to server. attempt: %d\n", this-> connect_trys);
				goto skip;
			}

			if (udp_stream.init(__server_ipaddr, 10197) == -1) {
				this-> server_connection = false;
				fprintf(stderr, "failed to connect to server. attempt: %d\n", this-> connect_trys);
				goto skip;
			}
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
/*
int main() {
	mdl::firefly::uni_worker uni_worker;
	uni_worker.begin("192.168.0.100");
}
*/
