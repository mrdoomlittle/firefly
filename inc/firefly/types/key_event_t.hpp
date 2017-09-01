# ifdef __key__event__t__hpp
# define __key__event__t__hpp
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	uint_t key_code;
	uint_t key_sym;
} key_event_t;
}
}
}

# endif /*__key__event__t__hpp*/
