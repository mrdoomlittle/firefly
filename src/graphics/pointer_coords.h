# ifndef __ffly__pointer__coords__h
# define __ffly__pointer__coords__h
# include <mdlint.h>
struct ffly_pointer_coords {
    mdl_i16_t root_xa, root_ya, wd_xa, wd_ya;
};

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace graphics {
typedef struct ffly_pointer_coords pointer_coords;
}
}
}
# endif
# endif /*__ffly__pointer__coords__h*/
