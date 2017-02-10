# include "x11_window.hpp"
# include "tcp_client.hpp"
# include "udp_client.hpp"
# define WIN_XA 640
# define WIN_YA 640
# include <chrono>
int main() {
	mdl::x11_window x11_window;
	mdl::tcp_client tcp_client;
	mdl::udp_client udp_client;
	tcp_client.init("192.168.0.100", 21298);
	if (udp_client.init("192.168.0.100", 21299)) while(1) {};

	x11_window.begin(WIN_XA, WIN_YA, "Firefly Engine");
	mdl::uint_t pix_count = (WIN_XA * WIN_YA) * 4;

	std::size_t tick_count = 0;
	auto begin = std::chrono::high_resolution_clock::now();

	while(true) {
		if (x11_window.pixels == nullptr) continue;
		tcp_client.write(x11_window.key_code);
		udp_client.recv(x11_window.pixels, pix_count);
/*
		for (std::size_t y = 0; y != 64; y ++) {
			for (std::size_t x = 0; x != (WIN_XA * 4); x ++) {
				x11_window.pixels[x + (y * (WIN_XA * 4) )] = 255;
			}
		}
	}
*/

		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(now - begin);
		std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		if (time_span.count() >= 1000) {
			std::cout << "FPS: " << tick_count << std::endl;
			tick_count = 0;

			begin = std::chrono::high_resolution_clock::now();
		} else tick_count ++;
	}
}
