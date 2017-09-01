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
# include "types/err_t.h"
# include "system/errno.h"
# include "types/init_opt_t.hpp"
# include "graphics/window.hpp"
# include "types/layer_info_t.hpp"
# include "system/event.hpp"
# include "ffly_system.hpp"
# include "ffly_graphics.hpp"
# include "entity_manager.hpp"
# include "types/err_t.h"
# include "system/smem_buff.h"
# include "types/event_t.hpp"
# ifdef __WITH_OBJ_MANAGER
#	include "obj_manager.hpp"
# endif

# ifdef ROOM_MANAGER
#	include "room_manager.hpp"
# endif
# include "firefly.hpp"
# ifdef __WITH_UNI_MANAGER
#	include "uni_manager.hpp"
#   include "types/uni_prop_t.hpp"
# endif
# include "types/id_t.h"
# include "system/io.h"
namespace mdl { class ffly_client
{
	public:
# ifndef __WITH_UNI_MANAGER
	ffly_client(u16_t __wd_xa_len, u16_t __wd_ya_len) : wd_xa_len(__wd_xa_len), wd_ya_len(__wd_ya_len) {}
# else
	ffly_client(u16_t __wd_xa_len, u16_t __wd_ya_len, firefly::types::uni_prop_t uni_props)
	: wd_xa_len(__wd_xa_len), wd_ya_len(__wd_ya_len), uni_manager(uni_props.xaxis_len, uni_props.yaxis_len, uni_props.zaxis_len) {}
# endif
	firefly::types::err_t connect_to_server(int& __sockd);
	firefly::types::err_t send_client_info();
	firefly::types::err_t recv_cam_frame();

	~ffly_client() {
		printf("ffly_client has safly shutdown.\n");
	}

	/* forward all events e.g. window, obj, etc. to the core event queue.
	*/
	firefly::types::err_t forward_events();

	void cu_clean() {
		cudaDeviceReset();
	}

	firefly::types::err_t de_init();

	void shutdown();
	struct portal_t {
		firefly::types::err_t init(ffly_client *__ffc_ptr) {
			this->ffc_ptr = this->_this = __ffc_ptr;
			this->pixbuff = __ffc_ptr->window.get_pixbuff();
			this->wd_handle = &__ffc_ptr->window.wd_handle;
			return FFLY_SUCCESS;
		}

		uint_t fps_count() {return _this->curr_fps;}

		firefly::types::err_t connect_to_server(char const *__addr, u16_t __portno, uint_t __layer_id) {
			if (_this->layer.does_layer_exist(__layer_id)) {
				fprintf(stderr, "error the layer for the camera does not exist.\n");
				return FFLY_FAILURE;
			}

			firefly::types::layer_info_t layer_info = _this->layer.get_layer_info(__layer_id);

			if (layer_info.xaxis_len != _this->cam_xlen || layer_info.yaxis_len != _this->cam_ylen) {
				fprintf(stderr, "error layer size does not match the camera size.\n");
				return FFLY_FAILURE;
			}

			_this->server_ipaddr = __addr;
			_this->server_portno = __portno;
			_this->cam_layer_id = __layer_id;

			return FFLY_SUCCESS;
		}

		bool server_connected() {return _this->server_connected;}
		firefly::types::pixmap_t pixbuff = nullptr;

		decltype(firefly::graphics::window::wd_handle) *wd_handle;
		ffly_client *_this;
		ffly_client *ffc_ptr;
	};

	firefly::types::__id_t bse_layer_id = 0;
	firefly::layer_manager layer;

	char const *server_ipaddr = nullptr;
	u16_t server_portno = 0;
	bool server_connected = false;

	firefly::types::err_t init(firefly::types::init_opt_t __init_options);

	firefly::types::err_t begin(char const *__frame_title,
		void (* __extern_loop)(boost::int8_t, portal_t*, void *), void *__this
	);


	// window
	firefly::graphics::window window;

//	bool poll_event(firefly::system::event& __event);

	bool static to_shutdown;

//	firefly::system::event *event = nullptr;
	portal_t portal;

	uint_t cam_layer_id = 0;
	uint_t curr_fps = 0;

	uint_t cam_xlen = 0, cam_ylen = 0;
	uint_t cam_pm_size = 0;
	uint_t connect_trys = 0;

	firefly::asset_manager asset_manager;
# ifdef __WITH_OBJ_MANAGER
	firefly::obj_manager *obj_manager = nullptr;
	void manage_objs() {
		if (this->obj_manager == nullptr) return;
		this->obj_manager->manage();
	}
# endif

# ifdef ROOM_MANAGER
	firefly::types::id_t bse_room_id;
	firefly::room_manager room_manager;
# endif

# ifdef __WITH_UNI_MANAGER
	firefly::uni_manager uni_manager;
# endif

	firefly::entity_manager entity_manager;
	private:
	firefly::types::init_opt_t init_options;
	firefly::types::client_info_t client_info;

	firefly::networking::tcp_client tcp_stream;
	firefly::networking::udp_client udp_stream;
	u16_t const wd_xa_len, wd_ya_len;
};
}

# endif /*__ffly__client__hpp*/

