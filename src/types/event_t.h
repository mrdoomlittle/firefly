# ifndef __ffly__event__t__h
# define __ffly__event__t__h
# include <mdlint.h>
typedef struct {
	mdl_uint_t kind;
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
