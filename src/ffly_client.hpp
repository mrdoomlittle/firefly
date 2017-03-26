# ifndef __ffly__client__hpp
# define __ffly__client__hpp
# include "networking/tcp_client.hpp"
# include "networking/udp_client.hpp"
# include <chrono>
# include <signal.h>
# include "draw.hpp"
# include <boost/cstdint.hpp>
# include "types/client_info_t.hpp"
# include "types/player_info_t.hpp"
# include "asset_manager.hpp"
# include "layer_manager.hpp"
# include <serializer.hpp>
# include <cuda_runtime.h>
# include "defaults.hpp"
# include <errno.h>
# include "system/errno.h"
# include "types/init_opt_t.hpp"
# include "graphics/window.hpp"
# include "types/layer_info_t.hpp"
# include "system/event.hpp"

# ifdef OBJ_MANAGER
#	include "obj_manager.hpp"
# endif
# ifdef UNI_MANAGER
#	 include "uni_manager.hpp"
# endif
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

	boost::int8_t de_init();

	typedef struct {
		uint_t fps_count() {
			return _this-> curr_fps;
		}

		bool poll_event(firefly::system::event& __event) {
			return this-> _this-> poll_event(__event);
		}

		boost::int8_t connect_to_server(char const *__addr, boost::uint16_t __portno, uint_t __layer_id) {
			if (_this-> layer.does_layer_exist(__layer_id)) {
				fprintf(stderr, "error the layer for the camera does not exist.\n");
				return FFLY_FAILURE;
			}

			firefly::types::layer_info_t layer_info = _this-> layer.get_layer_info(__layer_id);

			if (layer_info.xaxis_len != _this-> cam_xlen || layer_info.yaxis_len != _this-> cam_ylen) {
				fprintf(stderr, "error layer size does not match the camera size.\n");
				return FFLY_FAILURE;
			}

			_this-> server_ipaddr = __addr;
			_this-> server_portno = __portno;
			_this-> cam_layer_id = __layer_id;

			return FFLY_SUCCESS;
		}

		bool server_connected() {
			return _this-> server_connected;
		}

		firefly::types::pixmap_t pixbuff = nullptr;

		mdl::ffly_client *_this;
	} portal_t;

	firefly::layer layer;

	char const *server_ipaddr = nullptr;
	boost::uint16_t server_portno = 0;
	bool server_connected = false;

	bool _to_shutdown = false;

	boost::int8_t init(firefly::types::init_opt_t __init_options);

	boost::uint8_t begin(char const *__frame_title,
		void (* __extern_loop)(boost::int8_t, portal_t*, void *), void *__this
	);

	firefly::graphics::window window;

	bool poll_event(firefly::system::event& __event) {
		static bool init = false;
		if (init) {
			__event.key_code = 0x0;
			init = false;
			return false;
		}

		if (this-> window.wd_handler.key_press) {
			__event.key_code = this-> window.wd_handler.key_code;
			__event.event_type = firefly::system::event::KEY_PRESSED;
		} else if(!this-> window.wd_handler.key_press) {
			__event.key_code = this-> window.wd_handler.key_code;
			__event.event_type = firefly::system::event::KEY_RELEASED;
		} else {
			__event.event_type = firefly::system::event::NULL_EVENT;
			return false;
		}

		init = true;
		return true;
	}

	portal_t portal;

	uint_t cam_layer_id = 0;
	uint_t curr_fps = 0;
	uint_t fps_counter = 0;

	uint_t cam_xlen = 0, cam_ylen = 0;
	uint_t cam_pm_size = 0;
	uint_t connect_trys = 0;

	firefly::asset_manager asset_manager;
# ifdef OBJ_MANAGER
	firefly::obj_manager *obj_manager = nullptr;
	void manage_objs() {
		if (this-> obj_manager == nullptr) return;
		this-> obj_manager-> manage();
	}
# endif
# ifdef UNI_MANAGER
	firefly::uni_manager uni_manager(UNI_CHUNK_XS, UNI_CHUNK_YS, UNI_CHUNK_ZS);
# endif
	private:
	firefly::types::init_opt_t init_options;
	firefly::types::client_info_t client_info;

	firefly::networking::tcp_client tcp_stream;
	firefly::networking::udp_client udp_stream;
	uint_t const win_xlen = 0, win_ylen = 0;
};
}

# endif /*__ffly__client__hpp*/

