# ifndef __font__hpp
# define __font__hpp
# include <ft2build.h>
# include FT_FREETYPE_H
# include "system/errno.h"
# include "types/bitmap_t.hpp"
# include <strcmb.hpp>
# include <string.h>
# include <math.h>
# include "types/coords_t.hpp"
# include "memory/mem_alloc.h"
namespace mdl {
namespace firefly {
class font {
	public:
	boost::int8_t init() {
		if (FT_Init_FreeType(&this-> library)) {
			return FFLY_FAILURE;
		}
 
		return FFLY_SUCCESS;
	}

	boost::int8_t load_font(char const *__filedir, char const *__filename) {
		char *file_pth = strcmb(const_cast<char *>(__filedir), const_cast<char *>(__filename), STRCMB_FREE_NONE);

		if (FT_New_Face(library, file_pth, 0, &this-> face)) {
			return FFLY_FAILURE;
		}

		return FFLY_SUCCESS;
	}

	void set_size(std::size_t __size) {
		FT_Set_Pixel_Sizes(face, 0, __size);
		this-> size = __size;
	}

	types::bitmap_t make_bitmap(char const *__text, std::size_t __spacing, uint_t __xaxis_len, uint_t __yaxis_len) {
		firefly::types::bitmap_t bitmap = (firefly::types::bitmap_t)memory::mem_alloc(__xaxis_len * __yaxis_len);
		bzero(bitmap, __xaxis_len * __yaxis_len);

		std::size_t xoffset = 0;
		for (char const *text = __text;*text != '\0'; text++) {
			if (FT_Load_Char(this-> face, *text, FT_LOAD_RENDER)) {
				return nullptr;
			}

			FT_GlyphSlot g = this-> face-> glyph;
			FT_Bitmap bm = g-> bitmap;

			for (std::size_t y = 0; y != bm.rows; y ++) {
				for (std::size_t x = 0; x != bm.width; x ++) {
					std::size_t pos = (x + xoffset) + (y * __xaxis_len);
					std::size_t pos2 = x + (y * bm.width);
					bitmap[pos] = bm.buffer[pos2];
				}
			}
			xoffset += bm.width + __spacing;
		}
		this-> spacing = __spacing;
		this-> xaxis_len = __xaxis_len;
		this-> yaxis_len = __yaxis_len;
		this-> text_len = strlen(__text);
		return bitmap;
	}

	types::coords_t<> center() {
		types::coords_t<> cc = { (uint_t)(round(this-> xaxis_len/2) - round(((this-> text_len * (this-> size + this-> spacing)) - this-> spacing ))/2), (uint_t)(round((this-> yaxis_len/this-> size)/2) * 16), 0};
		return cc;
	}

	private:
	std::size_t text_len, spacing, size;
	uint_t xaxis_len, yaxis_len;
	FT_Face face;
	FT_Library library;
};
}
}

# endif /*__font__hpp*/
