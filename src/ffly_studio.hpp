# ifndef __ffly__studio__hpp
# define __ffly__studio__hpp
# include <boost/cstdint.hpp>
# include "types/init_opt_t.hpp"
# include "graphics/window.hpp"
# include "system/errno.h"
# include "gui/btn_manager.hpp"
# include "memory/alloc_pixmap.hpp"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include <eint_t.h>
# include <ft2build.h>
# include "types/skelmap_info_t.hpp"
# include "graphics/draw_skelmap.hpp"
# include "graphics/draw_bitmap.hpp"
# include "types/colour_t.hpp"
# include "types/bitmap_t.hpp"
# include <cuda_runtime.h>
# include "pulse_audio.hpp"
# include FT_FREETYPE_H
# include "font.hpp"
# include "maths/rotate_point.hpp"
# include "system/stop_watch.hpp"
# include "room_manager.hpp"
# include "types/id_t.h"
# include "gui/btn.hpp"
# include "maths/find_center.hpp"
# include "types/btn_t.hpp"
# include "asset_manager.hpp"
# include "graphics/draw_pixmap.hpp"
# include "graphics/draw_outline.hpp"
# include "graphics/png_loader.hpp"
# include "firefly.hpp"
# include "types/err_t.h"
# include <string.h>
# include <math.h>
# include "graphics/colour_blend.hpp"
# include "memory/mem_init.h"
# include "types/pixmap_t.h"
# include "gui/window.hpp"
# include "data/scale_pixmap.hpp"
# include "skel_creator.hpp"
# include "system/io.hpp"
namespace mdl {
class ffly_studio {
	public:
	ffly_studio(u16_t __wd_xaxis_len, u16_t __wd_yaxis_len)
	: wd_xaxis_len(__wd_xaxis_len), wd_yaxis_len(__wd_yaxis_len) {}

	firefly::types::err_t init(firefly::types::init_opt_t __init_options);
	firefly::types::err_t de_init();
	firefly::types::err_t begin(char const *__frame_title);

	firefly::types::id_t skelc_room_id = nullptr;
	firefly::types::id_t bse_room_id = nullptr;

	void chnage_room(firefly::types::id_t __room_id) {
		this-> _room_manager.change_room(__room_id);}

	struct {
		firefly::types::btn_t *exit_btn, *skelc_btn;
	} bse_room;

	struct {
		firefly::types::btn_t *main_menu_btn;
	} skelc_room;

	bool static to_shutdown;
	private:
	firefly::graphics::window window;
	firefly::room_manager _room_manager;
	firefly::asset_manager asset_manager;
	u16_t const wd_xaxis_len, wd_yaxis_len;
};
}

# endif /*__ffly__studio__hpp*/
