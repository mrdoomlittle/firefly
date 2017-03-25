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
# include <eint_t.hpp>
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
namespace mdl {
class ffly_studio {
	public:
	ffly_studio(boost::uint16_t __wd_xaxis_len, boost::uint16_t __wd_yaxis_len)
	: wd_xaxis_len(__wd_xaxis_len), wd_yaxis_len(__wd_yaxis_len) {}

	boost::int8_t init(firefly::types::init_opt_t __init_options);
	boost::int8_t begin(char const *__frame_title);
	private:
	boost::uint16_t const wd_xaxis_len, wd_yaxis_len;
};
}

# endif /*__ffly__studio__hpp*/
