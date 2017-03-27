# ifndef __room__manager__hpp
# define __room__manager__hpp
# include <boost/cstdint.hpp>
# include <eint_t.hpp>
# include <set>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "gui/btn_manager.hpp"
# include "types/pixmap_t.h"
# include "types/coords_t.hpp"
# include "graphics/window.hpp"
namespace mdl {
namespace firefly {
class room_manager {
	public:
	typedef struct {
		uint_t *id = nullptr;
	} room_info_t;

	typedef struct {
		uint_t pb_xaxis_len, pb_yaxis_len;
		types::pixmap_t pixbuff = nullptr;
		gui::btn_manager *btn_manager;
	} room_data_t;

	boost::int8_t create_btn();

	bool id_inuse(uint_t *__room_id) {
		if (this-> unused_ids.find(__room_id) == this-> unused_ids.end()) return false;
		return true;
	}

	room_info_t get_room_info(uint_t *__room_id) noexcept {
		if (id_inuse(__room_id))
			return this-> _room_info[*__room_id];
	}
	room_info_t& room_info(uint_t *__room_id) {
		return this-> _room_info[*__room_id];
	}

	room_data_t get_room_data(uint_t *__room_id) noexcept {
		if (id_inuse(__room_id))
			return this-> _room_data[*__room_id];
	}
	room_data_t& room_data(uint_t *__room_id) {
		return this-> _room_data[*__room_id];
	}

	boost::int8_t add_room(uint_t*& __room_id, bool __overwrite);
	boost::int8_t rm_room(uint_t *__room_id, bool __hard = false);

	boost::int8_t change_room(uint_t *__room_id) {
		if (id_inuse(__room_id)) {
			this-> curr_room_id = __room_id;
			return FFLY_SUCCESS;
		}
		return FFLY_FAILURE;
	}

	boost::int8_t handle_room(uint_t *__room_id);

	boost::int8_t init(graphics::window *__window = nullptr) {
		if (__window == nullptr) {
			fprintf(stderr, "room_manager: you've passed a empty pointer for the window thru init.\n");
			return FFLY_FAILURE;
		} else
			this-> window = __window;
		return FFLY_SUCCESS;
	}

	boost::int8_t de_init();

	boost::int8_t manage(uint_t *__room_id = nullptr);
	private:
	types::coords_t<boost::int16_t> wd_coords;
	types::coords_t<boost::uint16_t> mouse_coords;
	graphics::window *window = nullptr;
	types::pixmap_t pix_buff = nullptr;
	uint_t *curr_room_id = NULL;
	uint_t room_count = 0;
	std::set<uint_t *> unused_ids;
	room_info_t *_room_info = NULL;
	room_data_t *_room_data = NULL;
};

}
}

# endif /*__room__manager__hpp*/
