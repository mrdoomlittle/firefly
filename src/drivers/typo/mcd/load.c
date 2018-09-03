# include "../mcd.h"
# include "../../../typo/memalloc.h"
# include "../../../linux/unistd.h"
# include "../../../typo.h"
# include "../../../system/io.h"
# include "../../../hexdump.h"
# include "../../../typo/film.h"
extern struct typo_face *mcd_face;

struct glyph {
	ff_u16_t idx;
	ff_u32_t code;
	ff_uint_t len;
};

struct header {
	ff_u32_t glyph_table;
	ff_uint_t glyph_c;
	ff_uint_t width, height;
};

void
mcd_load(struct ffly_typo_sheet *__sheet, struct typo_face *__face) {
	ffly_printf("mcd load.\n");
	mcd_face = __face;
	struct header hdr;
	typo_read(&hdr, sizeof(struct header));
	__sheet->f_width = hdr.width;
	__sheet->f_height = hdr.height;
	typo_seek(hdr.glyph_table, SEEK_SET);
	ffly_printf("mcd header: glyph count: %u\n", hdr.glyph_c);

	if (hdr.glyph_c > 2) {
		return;
	}

	struct glyph *table;

	table = (struct glyph*)ffly_tmalloc(hdr.glyph_c*sizeof(struct glyph));
	typo_read(table, hdr.glyph_c*sizeof(struct glyph));

	struct glyph *g;
	ff_uint_t i;

	i = 0;
	for(;i != hdr.glyph_c;i++) {
		ffly_printf("loading glyph %u\n", i);
		g = table+i;
		typo_seek(g->code, SEEK_SET);
		void *code;

		code = ffly_tmalloc(g->len);
		typo_read(code, g->len);
		ffly_printf("reading code at : %u\n", g->code);

		struct typo_glyph *gg;

		gg = ffly_typo_glyph_new(g->idx);

		struct typo_film *film;

		film = typo_film_new();
		typo_film_insert(film, code, g->len);
		ffly_hexdump(code, g->len);
		ffly_tfree(code);
		gg->film = film;
	}
	ffly_tfree(table);
}

void
mcd_load_glyph(ff_u16_t __index) {
	mcd_face->glyph = *(glyph_table+__index);	
}

