# include "../mcd.h"
# include "../../../typo/memalloc.h"
# include "../../../linux/unistd.h"
# include "../../../typo.h"
# include "../../../system/io.h"
# include "../../../hexdump.h"
# include "../../../tape.h"
# include "../../../dep/mem_cpy.h"
# include "../../../typo/raise.h"
# include "../../../typo/mcd/struc.h"
extern struct typo_face *mcd_face;


void static
load_segs(struct segment *__segs, ff_uint_t __n) {
	struct segment *s;

	s = __segs;
	ffly_printf("segment load: %u, %u, %u\n", s->src, s->dst, s->size);

	typo_seek(s->src, SEEK_SET);
	typo_read(raise_stack+s->dst, s->size);

}

void
mcd_load(struct ffly_typo_sheet *__sheet, struct typo_face *__face) {
	ffly_printf("mcd load.\n");
	mcd_face = __face;
	struct header hdr;
	typo_read(&hdr, sizeof(struct header));
	__sheet->f_width = hdr.width;
	__sheet->f_height = hdr.height;
	typo_seek(hdr.glyph_table, SEEK_SET);
	ffly_printf("mcd header:\n\tglyph table: %u\n\tglyph count: %u\n\twidth: %u\n\theight: %u\n", hdr.glyph_table, hdr.glyph_c, hdr.width, hdr.height);
//	if (hdr.glyph_c > 2) {
//		return;
//	}

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
		ffly_printf("reading code at : %u, span: %u\n", g->code, g->len);

		struct typo_glyph *gg;

		gg = ffly_typo_glyph_new(g->idx);

		struct ffly_tape *tape;

		tape = ffly_tape_new();
		ffly_tape_insert(tape, code, g->len);
		ffly_hexdump(code, g->len);
		ffly_tfree(code);
		gg->tape = tape;
	}
	ffly_tfree(table);

	/*
		load segments into stack
	*/
	struct segment segs;
	typo_seek(hdr.seg, SEEK_SET);
	typo_read(&segs, sizeof(struct segment));
	load_segs(&segs, 1);
}

void
mcd_load_glyph(ff_u16_t __index) {
	mcd_face->glyph = *(glyph_table+__index);	
}

