# ifndef __system__event__hpp
# define __system__event__hpp
# include <eint_t.hpp>
# include <queue>
# include "../data/pair.hpp"
# include "../types/btn_event_t.hpp"
namespace mdl {
namespace firefly {
namespace system {
namespace xevent {
enum event_id {
	KEY_PRESSED,
	KEY_RELEASED,
	MOUSE_CLICK,
	MSE_PRESSED,
	MSE_RELEASED,
	BTN_PRESSED,
	BTN_RELEASED,
	BTN_HOVER,
	BTN_HOVERING,
	BTN_PRESS
};

typedef enum event_id event_id_t;

types::err_t event_add(event_id_t __event_id, void *__event_data);
void* nxt_event(event_id_t& __event_id);
void event_pop();

uint_t max_queue_size();
void max_queue_size(uint_t __queue_size);

std::queue<data::pair<event_id_t, void*>> extern event_queue;
}

class event {
	public:
	typedef struct {
		int key_code;
		uint_t event_type;
		void *data;
	} event_t;

	void *data;
	uint_t get_keycode() {

	}

	uint_t key_code;
	uint_t event_type;

	void event_handler();

	void queue_add(event_t __event) {
		this-> last_event = __event.event_type;
		this-> event_queue.push(__event);
	}

	void queue_rm() {
		this-> event_queue.pop();
	}

	bool queue_empty() {
		return this-> event_queue.empty();
	}

	event_t nxt_event() {
		event_t tmp = this-> event_queue.front();
		this-> event_queue.pop();
	}

	uint_t last_event;

	enum {
		NULL_EVENT,
		KEY_PRESSED,
		KEY_RELEASED,
		MOUSE_CLICK,
		MSE_PRESSED,
		MSE_RELEASED,
		BTN_PRESSED,
		BTN_RELEASED,
		BTN_HOVER,
		BTN_HOVERING,
		BTN_PRESS
	};

	enum {
		WD_KEY_W = 25,
		WD_KEY_A = 38,
		WD_KEY_S = 39,
		WD_KEY_D = 40
	};

	std::queue<event_t> event_queue;
};
}
}
}

# endif /*__system__event__hpp*/
