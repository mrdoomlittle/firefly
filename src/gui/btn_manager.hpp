# ifndef __ffly__btn__manager__hpp
# define __ffly__btn__manager__hpp
# include <mdlint.h>
# include "../types/pixelmap_t.h"
namespace mdl {
namespace firefly {
namespace gui {
class btn_manager {
    btn_manager(i16_t *__wd_xa, i16_t *__wd_ya) : wd_xa(__wd_xa), wd_ya(__wd_ya) {}
    types::err_t draw(types::pixelmap_t __pixelbuff, u16_t __width, u16_t __height);
    types::err_t manage();

    i16_t *wd_xa = nullptr, *wd_ya = nullptr;
};
}
}
}


# endif /*__ffly__btn__manager__hpp*/
