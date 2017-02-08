# include "src/x11_window.hpp"
# include "src/udp_client.hpp"
# define WIN_XA 640
# define WIN_YA 640

int main() {
	mdl::x11_window x11_window;
	mdl::udp_client udp_client;
	udp_client.init("192.168.0.10", 21299);

	x11_window.begin(WIN_XA, WIN_YA, "Firefly Engine");
	mdl::uint_t pix_count = (WIN_XA * WIN_YA) * 4;

	while(true) {
		udp_client.recv(x11_window.pixels, pix_count);
	}
}
