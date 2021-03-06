# ifndef __ffly__client__hpp
# define __ffly__client__hpp
# include "types/err_t.h"
# include "system/io.h"
# include "types/bool_t.h"
# include "system/errno.h"
# include "graphics/window.hpp"
# include "system/event.h"
# include "types/byte_t.h"

# include "layer_manager.hpp"
# include "room_manager.hpp"
# include "asset_manager.h"
# include "graphics/pointer_coords.h"
namespace mdl { class ffly_client
{
	public:
	firefly::types::bool_t static to_shutdown;
	typedef struct {
		firefly::types::err_t init(ffly_client *__ffc_p) {
			this->ffc_p = __ffc_p;
		}

		void shutdown() {
			this->ffc_p->shutdown();
		}

		uint_t get_fps() {
			if (!this->ffc_p) return 0;
			return this->ffc_p->get_fps();
		}

		ffly_client* get_ffc_p(){return this->ffc_p;}
		firefly::types::byte_t* frame_buff() {return this->ffc_p->frame_buff();}
		private:
		ffly_client *ffc_p = nullptr;
	} portal_t;

	firefly::types::err_t init(u16_t __wd_width, u16_t __wd_height, char const *__wd_title);
	firefly::types::err_t de_init();
	firefly::types::err_t begin(ffly_err_t(*__extern_loop)(i8_t, portal_t*, void*), void *__this);
	void shutdown();
	uint_t get_fps() {return this->fps;}

	void free_wd_event(firefly::types::event_t *__event) {
		this->window.free_event(__event);
	}

	firefly::types::byte_t* frame_buff() {
		return this->window.frame_buff();
	}

	firefly::layer_manager layer_m;
	firefly::room_manager room_m;
	firefly::asset_manager asset_m;

	portal_t portal;
	uint_t fps = 0, frame_c = 0;
	firefly::graphics::window window;
	u16_t wd_width, wd_height;
/*
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

	 forward all events e.g. window, obj, etc. to the core event queue.
	
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

	types::bool_t static to_shutdown;

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
*/
};
}

# endif /*__ffly__client__hpp*/

