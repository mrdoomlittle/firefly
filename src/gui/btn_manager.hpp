# ifndef __ffly__btn__manager__hpp
# define __ffly__btn__manager__hpp
# include <mdlint.h>
# include "../types/pixelmap_t.h"
# include "../system/vec.h"
# include "btn.h"
# include "../types/id_t.h"
namespace mdl {
namespace firefly {
namespace gui {
class btn_manager {
    public:
    btn_manager() : buttons(VEC_AUTO_RESIZE) {}
    types::err_t init(i16_t *__pt_xa, i16_t *__pt_ya) {
        this->pt_xa = __pt_xa;
        this->pt_ya = __pt_ya;
    }

    types::err_t draw(types::pixelmap_t __pixelbuff, u16_t __width, u16_t __height);
    types::err_t manage();

    types::err_t add(u16_t __xa, u16_t __ya, types::pixelmap_t __texture, u16_t __width, u16_t __height, types::id_t& __id);
    btn& get(types::id_t __id) {
        return this->buttons[*__id];
    }

    system::vec<btn> buttons;
    mdl_uint_t no_btns = 0;
    i16_t *pt_xa = nullptr, *pt_ya = nullptr;
};
}
}
}


# endif /*__ffly__btn__manager__hpp*/
