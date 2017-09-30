# ifndef __ffly__map__h
# define __ffly__map__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "vec.h"
# define FFLY_MAP_SIZE 0xFF
struct ffly_map {
	struct ffly_vec **table;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_map_init(struct ffly_map*);
ffly_err_t ffly_map_de_init(struct ffly_map*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__map__h*/
