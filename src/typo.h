# ifndef __ffly__typo__h
# define __ffly__typo__h
# include "ffint.h"
# include "typo/film.h"
// put within define
struct typo_point {
	ff_int_t x, y;
};

struct typo_glyph {
	struct typo_film *film;
};

struct typo_face {
	ff_uint_t width, height;
	struct typo_glyph *glyph;
};

struct ffly_typo_sheet;
struct typo_driver {
	char const *name;
	void(*load)(struct ffly_typo_sheet*, struct typo_face*);
	void(*load_glyph)(ff_u16_t);
};


extern struct typo_glyph **glyph_table;
enum {
	_ffly_typo_mcd
};

typedef struct ffly_typo_sheet {
	ff_uint_t f_width, f_height;
} *ffly_typo_sheetp;

extern void(*typo_read)(void*, ff_uint_t);
extern void(*typo_write)(void*, ff_uint_t);
extern void(*typo_seek)(ff_u64_t, ff_u32_t);

struct typo_glyph* ffly_typo_glyph_new(ff_u16_t);
void ffly_typo_init(ffly_typo_sheetp, ff_u8_t);
void ffly_typo_face(ffly_typo_sheetp, char const*, struct typo_face*);
//void ffly_typo_face_size(ffly_typo_sheetp, struct typo_face*, ff_uint_t, ff_uint_t);

# endif /*__ffly__typo__h*/
