# ifndef __ffly__camera__hpp
# define __ffly__camera__hpp
# include <mdlint.h>
# include "../types/err_t.h"
# include "../types/pixelmap_t.h"
# include "../uni.h"
# include "../types/off_t.h"
namespace mdl {
namespace firefly {
namespace ui {
class camera {
    void init(u16_t __width, u16_t __height) {
        this->width = __width;
        this->height = __height;
    }
    types::err_t draw(uni& __uni, uint_t __xa, uint_t __ya, types::pixelmap_t __pixelbuff, uint_t __width, uint_t __height);

    void set_coords(uint_t __xa, uint_t __ya, uint_t __za) {
        this->xa = __xa;
        this->ya = __ya;
        this->za = __za;
    }

    void set_xa(uint_t __xa) {this->xa=__xa;}
    void set_ya(uint_t __ya) {this->ya=__ya;}
    void set_za(uint_t __za) {this->za=__za;}

    u16_t width, height;
    uint_t xa, ya, za;
};
}
}
}

# endif /*__ffly__camera__hpp*/
