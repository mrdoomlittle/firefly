# include "../../../types.h"
# include "../concoct.h"
# include "../../../typo.h"
# define N 34
static struct typo_point points[] = {
// LETTER: A
	{29, 29},
	{16, 2},
	{2, 29},
	{6, 29},
	{13, 16}, // mid inner
	{13, 18},

	{16, 8},
	{18, 16}, // mid inner
	{18, 18},

	{25, 29},
	{29, 29},

	{13, 16},
	{18, 16},
	{18, 18},
	{13, 18},
	{13, 16},

// ZERO
	{6, 2},
	{25, 2},
	{29, 6},
	{29, 25},
	{25, 29},
	{6, 29},
	{2, 25},
	{2, 6},
	{6, 2},

	{10, 6},
	{21, 6},
	{25, 10},
	{25, 21},
	{21, 25},

	{10, 25},
	{6, 21},
	{6, 10},
	{10, 6}
};

# include "../../../dep/mem_cpy.h"
# include "../../../linux/unistd.h"
# include "../../../linux/stat.h"
# include "../../../linux/fcntl.h"
static int fd;
void static _write(void *__buf, ff_uint_t __size, ff_u32_t __offset) {
	pwrite(fd, __buf, __size, __offset);
}

ff_err_t ffmain(int __argc, char const *__argv[]) { 
	ffly_mcd_cc_write = _write;
	ffly_mcd_cc_prime();
	struct cc_glyph *g[0x100];
	g['A'] = ffly_mcd_cc_glyph_new();
	g['0'] = ffly_mcd_cc_glyph_new();

	struct cc_segment *s;
	s = ffly_mcd_cc_seg_new();
	s->text.dst = 0;
	struct typo_point *p;
	p = (struct typo_point*)ffly_mcd_cc_seg_alloc(s, N*sizeof(struct typo_point));
	ffly_mem_cpy(p, points, N*sizeof(struct typo_point));

	g['A']->tape = ffly_tape_new();
	g['0']->tape = ffly_tape_new();
	ff_u8_t A_code[] = {
		0x01,
		0x01,
		0x02,
		0x00, 0x00, // number of points
		0x00, 0x00, // address of points within stack 
		0x02,
		0x00, 0x00,
		0x00, 0x00,
		0x01,
		0x02
	};

	*(ff_u16_t*)(A_code+3) = 11;
	*(ff_u16_t*)(A_code+5) = seg_off(p, s);
	*(ff_u16_t*)(A_code+8) = 5;
	*(ff_u16_t*)(A_code+10) = seg_off(p, s)+(11*sizeof(struct typo_point));
	ff_u32_t off;
	off = (11+5)*sizeof(struct typo_point);

	ff_u8_t ZERO_code[] = {
		0x01,
		0x01,
		0x02,
		0x00, 0x00,
		0x00, 0x00,
		0x02,
		0x00, 0x00,
		0x00, 0x00,
		0x01,
		0x02
	};


	*(ff_u16_t*)(ZERO_code+3) = 9;
	*(ff_u16_t*)(ZERO_code+5) = seg_off(p, s)+off;
	*(ff_u16_t*)(ZERO_code+8) = 9;
	*(ff_u16_t*)(ZERO_code+10) = seg_off(p, s)+off+(9*sizeof(struct typo_point));

	ffly_tape_insert(g['A']->tape, A_code, sizeof(A_code));
	ffly_tape_insert(g['0']->tape, ZERO_code, sizeof(ZERO_code));
	fd = open ("test.mcd", O_RDWR|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);


	ffly_mcd_cc_face_size(32, 32);
	ffly_mcd_cc();
	close(fd);
}
