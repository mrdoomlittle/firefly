# ifndef __room__manager__hpp
# define __room__manager__hpp
# include <boost/cstdint.hpp>
# include <eint_t.hpp>
# include <set>
# include <queue>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "gui/btn_manager.hpp"
# include "types/pixmap_t.h"
# include "types/coords_t.hpp"
# include "graphics/window.hpp"
# include "types/btn_t.hpp"
# include "types/btn_event_t.hpp"
# include <queue>
namespace mdl {
namespace firefly {
class room_manager {
	public:
	room_manager();
	typedef struct {
		uint_t *id = nullptr;
	} room_info_t;

	typedef struct {
		uint_t pb_xaxis_len, pb_yaxis_len;
		types::pixmap_t pixbuff = nullptr;
		gui::btn_manager *btn_manager;
	} room_data_t;

	boost::int8_t create_btn(uint_t *__room_id, uint_t& __btn_id, types::pixmap_t __pixmap, types::_2d_coords_t<> __coords, uint_t __xaxis_len, uint_t __yaxis_len);

	bool id_inuse(uint_t *__room_id) {
		if (this-> unused_ids.find(__room_id) != this-> unused_ids.end()) return false;
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

	inline static gui::btn_manager& get_btn_manager(uint_t *__room_id, room_manager *__rm) {
		return *__rm-> _room_data[*__room_id].btn_manager;
	}

	void set_pixbuff(types::pixmap_t __pixbuff) {
		this-> pixbuff = __pixbuff;
	}

	types::btn_t* get_btn(uint_t *__room_id, uint_t __btn_id) {
		return this-> _room_data[*__room_id].btn_manager-> get_btn(__btn_id);
	}

	void static (* _btn_press)(uint_t, int, void *, uint_t *);
	void static (* _btn_hover)(uint_t, void *, uint_t *);

	uint_t static **_room_id;

	void static btn_pressf(uint_t __btn_id, int __mbtn_id, void *__voidptr);
	void static btn_hoverf(uint_t __btn_id, void *__voidptr);

	void (* btn_press(void (* __btn_press)(uint_t, int, void *, uint_t *)))(uint_t, int, void *);
	void (* btn_hover(void (* __btn_hover)(uint_t, void *, uint_t *)))(uint_t, void *);

	void set_glob_pb_xlen(uint_t __xaxis_len) {
		this-> pb_xaxis_len = __xaxis_len;
	}

	void set_glob_pb_ylen(uint_t __yaxis_len) {
		this-> pb_yaxis_len = __yaxis_len;
	}

	boost::int8_t add_room(uint_t*& __room_id, bool __overwrite);
	boost::int8_t rm_room(uint_t *__room_id, bool __hard = false);

	boost::int8_t change_room(uint_t *__room_id) {
		if (id_inuse(__room_id)) {
			this-> room_change.push(__room_id);
			return FFLY_SUCCESS;
		}
		return FFLY_FAILURE;
	}

	boost::int8_t draw_room(uint_t *__room_id = nullptr);

	boost::int8_t init(graphics::window *__window = nullptr, types::pixmap_t __pixbuff = nullptr) {
		if (__window == nullptr) {
			fprintf(stderr, "room_manager: you've passed a empty pointer for the window thru init.\n");
			return FFLY_FAILURE;
		} else
			this-> window = __window;

		if (__pixbuff != nullptr) this-> pixbuff = __pixbuff;

		return FFLY_SUCCESS;
	}

	boost::int8_t de_init();

	boost::int8_t manage(uint_t *__room_id = nullptr);

	bool use_glob_pb_size = false;
	std::queue<types::btn_event_t> btn_event_pool;
	private:
	types::coords_t<boost::int16_t> wd_coords;
	types::coords_t<boost::uint16_t> mouse_coords;
	graphics::window *window = nullptr;
	types::pixmap_t pixbuff = nullptr;

	uint_t pb_xaxis_len = 0, pb_yaxis_len = 0;
	uint_t *curr_room_id = NULL;
	uint_t room_count = 0;
	std::set<uint_t *> unused_ids;
	room_info_t *_room_info = NULL;
	room_data_t *_room_data = NULL;
	std::queue<uint_t *> room_change;
};

}
}

# endif /*__room__manager__hpp*/
