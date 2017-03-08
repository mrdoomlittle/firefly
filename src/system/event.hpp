# ifndef __system__event__hpp
# define __system__event__hpp
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace system {
class event {
	public:
	uint_t get_keycode() {

	}

	uint_t key_code;
	uint_t event_type;

	void event_handler();

	enum {
		NULL_EVENT,
		KEY_PRESSED,
		KEY_RELEASED,
		MOUSE_CLICK
	};

};
}
}
}

# endif /*__system__event__hpp*/
