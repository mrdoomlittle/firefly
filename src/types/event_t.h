# ifndef __ffly__event__t__h
# define __ffly__event__t__h
# include "../ffint.h"
typedef struct {
	ff_u8_t field;
	ff_u8_t kind;
	void *data;
} ffly_event_t;

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace types {
typedef ffly_event_t event_t;
}
}
}
# endif
# endif /*__ffly__event__t__h*/
