# include "src/udp_server.hpp"
# include <iostream>
# define WIN_XA 640 
# define WIN_YA 640
int main() {
	mdl::udp_server udp_server;
	udp_server.init(21299);
	mdl::uint_t pix_count = (WIN_XA * WIN_YA) * 4;
	boost::uint8_t * pixs = new boost::uint8_t[pix_count];
	memset(pixs, 100, pix_count);

	while(true) {
		udp_server.send(pixs, pix_count);
	}
}
