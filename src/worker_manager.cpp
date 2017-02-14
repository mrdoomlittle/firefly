# include "tcp_server.hpp"
# include "udp_server.hpp"
# include "serial.hpp"
# include "worker_info_t.hpp"
# include <iostream>
# include <boost/cstdint.hpp>
int main() {
	mdl::tcp_server	mcomm_pipe;
//	mdl::udp_server cam_stream;
/*
	worker_info_t worker_info;
	worker_info.chunk_xlen = 21;
	worker_info.chunk_ylen = 99;

	if (mcomm_pipe.init(21299)) return 1;
	int sock;
	mcomm_pipe.accept(sock);

	//if (cam_stream.init(10198)) return 1;

	serial archive('\0');
	boost::uint8_t outgoing[3] = {21, 2, 99};
	std::size_t size = archive.get_size(&worker_info);
	printf("size: %d\n", size);
	archive | 'r';
	archive.init(size);

	worker_info.arc(archive);
	mcomm_pipe.send(sock, archive.get_data(), size);
	return 0;
*/
}
