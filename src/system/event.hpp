# ifndef __system__event__hpp
# define __system__event__hpp
# include <eint_t.hpp>
# include <queue>
# include "../types/btn_event_t.hpp"
namespace mdl {
namespace firefly {
namespace system {
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
