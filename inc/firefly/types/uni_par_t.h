# ifndef __uni__par__t__h
# define __uni__par__t__h
# include "colour_t.h"
typedef struct {
	ffly_colour_t *colour;
} ffly_uni_par_t;

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace types {
typedef ffly_uni_par_t uni_par_t;
}
}
}
# endif
# endif /*__uni__par__t__h*/
