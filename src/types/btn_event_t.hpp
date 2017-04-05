# ifndef __btn__event__t__hpp
# define __btn__event__t__hpp
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	uint_t btn_id;
	int mbtn_id;
	uint_t event_type;
} btn_event_t;
}
}
}

# endif /*__btn__event__t__hpp*/
