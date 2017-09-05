# ifndef __btn__t__hpp
# define __btn__t__hpp
# include <mdlint.h>
# include "pixmap_t.h"
# include "bitmap_t.hpp"
# include "coords_t.h"
# include "colour_t.h"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	_2d_coords_t coords;
	_2d_coords_t text_coords;
	uint_t xaxis_len, yaxis_len;
	bool enabled, hover_enabled, mouse_hovering;
	bool press_enabled, already_pressed;
	void(*press_fptr)(uint_t, int, void*) = nullptr;
	void(*hover_fptr)(uint_t, void*) = nullptr;
	pixmap_t pixmap, texture;
	bitmap_t text_bm;
	colour_t text_colour;
	bool draw_text_bm = false;
	bool inde_tx_mem = false;
	void *voidptr = nullptr;
	bool voidptr_lock = false;
} btn_t;
}
}
}

# endif /*__btn__t__hpp*/
