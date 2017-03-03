# ifndef __ffly__client__hpp
# define __ffly__client__hpp
# include "graphics/x11_window.hpp"
# include "networking/tcp_client.hpp"
# include "networking/udp_client.hpp"
# include <chrono>
# include <signal.h>
# include "draw.hpp"
# include <boost/cstdint.hpp>
# include "types/client_info_t.hpp"
# include "types/player_info_t.hpp"
# include "asset_manager.hpp"
# include "layer.hpp"
# include <serializer.hpp>
# include <cuda_runtime.h>
# include "defaults.hpp"
# include <errno.h>
# include "types/status.hpp"
namespace mdl { class ffly_client
{
	public:
	ffly_client(uint_t __win_xlen, uint_t __win_ylen) : win_xlen(__win_xlen), win_ylen(__win_ylen) {}
	boost::int8_t connect_to_server(int& __sock);
	boost::int8_t send_client_info();
	boost::int8_t recv_cam_frame();

	~ffly_client() {
		printf("ffly_client has safly shutdown.\n");
		cudaDeviceReset();
	}

	void cu_clean() {
		cudaDeviceReset();
	}

	typedef struct {
		uint_t fps_count() {
			return _this-> curr_fps;
		}

		void connect_to_server(char const *__addr, boost::uint16_t __portno, uint_t __layer_id) {
			_this-> server_ipaddr = __addr;
			_this-> server_portno = __portno;
			_this-> cam_layer_id = __layer_id;
		}

		bool server_connected() {
			return _this-> server_connected;
		}

		boost::uint8_t *pixbuff = nullptr;

		mdl::ffly_client *_this;
	} portal_t;

	firefly::layer layer;

	char const *server_ipaddr = nullptr;
	boost::uint16_t server_portno = 0;
	bool server_connected = false;

	bool to_shutdown = false;
	boost::int8_t init();
	boost::uint8_t begin(char const *__frame_title, void (* __o)(boost::int8_t, portal_t*));

	portal_t portal;

	uint_t cam_layer_id = 0;
	uint_t curr_fps = 0;
	uint_t fps_counter = 0;

	uint_t cam_xaxis_len = 256, cam_yaxis_len = 256; 
	uint_t cam_frame_size = (cam_xaxis_len * cam_yaxis_len) * 4;
	uint_t connect_trys = 0;

	firefly::asset_manager asset_manager;
	private:
	firefly::types::client_info_t client_info;

	firefly::networking::tcp_client tcp_stream;
	firefly::networking::udp_client udp_stream;
	uint_t win_xlen = 0, win_ylen = 0;
};
}

# endif /*__ffly__client__hpp*/
