# include "btn_manager.hpp"
# include "../memory/mem_alloc.h"
mdl::firefly::types::err_t mdl::firefly::gui::btn_manager::add(u16_t __xa, u16_t __ya, types::pixelmap_t __texture, u16_t __width, u16_t __height, types::id_t& __id) {
    btn button;
    button.clear();
    button.set_xa(__xa);
    button.set_ya(__ya);
    button.set_texture(__texture);
    button.set_width(__width);
    button.set_height(__height);
    __id = (types::id_t)memory::mem_alloc(sizeof(types::__id_t));
    *__id = no_btns++;
    this->buttons.push_back(button);
}

mdl::firefly::types::err_t mdl::firefly::gui::btn_manager::draw(types::pixelmap_t __pixelbuff, u16_t __width, u16_t __height) {
    if (this->buttons.empty()) {

        return FFLY_SUCCESS;
    }

    system::vec<btn>::iterator itr = this->buttons.begin();
    while(itr <= this->buttons.end()) {
        // draw
        itr++;
    }
}

mdl::firefly::types::err_t mdl::firefly::gui::btn_manager::manage() {

}
