# ifndef __ffly__typo__mcd__struc__h
# define __ffly__typo__mcd__struc__h
# include "../../ffint.h"
struct glyph {
	ff_u16_t idx;
	ff_u32_t code;
	ff_uint_t len;
};

struct segment {
	ff_u16_t dst;
	ff_u16_t src;
	ff_u16_t size;
};

struct header {
	ff_u32_t glyph_table;
	ff_uint_t glyph_c;
	ff_uint_t width, height;
	ff_uint_t seg;
};

# endif /*__ffly__typo__mcd__struc__h*/
