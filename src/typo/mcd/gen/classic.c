# include "../../../types.h"
# include "../concoct.h"
# include "../../../typo.h"
#define SZ 256
# include "../../../tools/typo/test.h"

# include "../../../dep/mem_cpy.h"
# include "../../../linux/unistd.h"
# include "../../../linux/stat.h"
# include "../../../linux/fcntl.h"
static int fd;
void static _write(void *__buf, ff_uint_t __size, ff_u32_t __offset) {
	pwrite(fd, __buf, __size, __offset);
}

//# define ENABLE
#ifdef ENABLE
ff_err_t ffmain(int __argc, char const *__argv[]) { 
	ffly_mcd_cc_write = _write;
	ffly_mcd_cc_prime();
	struct cc_glyph *g[0x100];
	g['0'] = ffly_mcd_cc_glyph_new('0');
	g['1'] = ffly_mcd_cc_glyph_new('1');
	g['2'] = ffly_mcd_cc_glyph_new('2');

	struct cc_segment *s;
	s = ffly_mcd_cc_seg_new();
	s->text.dst = 0;
	struct typo_point *p;
	p = (struct typo_point*)ffly_mcd_cc_seg_alloc(s, N*sizeof(struct typo_point));
	ffly_mem_cpy(p, points, N*sizeof(struct typo_point));
	ff_uint_t off;
	off = 0;

	g['0']->tape = ffly_tape_new();
	g['1']->tape = ffly_tape_new();
	g['2']->tape = ffly_tape_new();

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
	*(ff_u16_t*)(ZERO_code+5) = seg_off(p, s);
	*(ff_u16_t*)(ZERO_code+8) = 9;
	*(ff_u16_t*)(ZERO_code+10) = seg_off(p, s)+(9*sizeof(struct typo_point));

	off+=18*sizeof(struct typo_point);

	ffly_tape_insert(g['0']->tape, ZERO_code, sizeof(ZERO_code));

	ff_u8_t ONE_code[] = {
		0x01,
		0x01,
		0x02,
		0x00, 0x00,
		0x00, 0x00,
		0x01,
		0x02
	};

	*(ff_u16_t*)(ONE_code+3) = 5;
	*(ff_u16_t*)(ONE_code+5) = seg_off(p, s)+off;
	off+=5*sizeof(struct typo_point);
	ffly_tape_insert(g['1']->tape, ONE_code, sizeof(ONE_code));

/*
	TWO
*/
	ff_u8_t TWO_code[] = {
		0x01,
		0x01,
		0x02,
		0x00, 0x00,
		0x00, 0x00,
		0x01,
		0x02
	};

	*(ff_u16_t*)(TWO_code+3) = 31;
	*(ff_u16_t*)(TWO_code+5) = seg_off(p, s)+off;
	
	ffly_tape_insert(g['2']->tape, TWO_code, sizeof(TWO_code));

	fd = open ("test.mcd", O_RDWR|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
	ffly_mcd_cc_face_size(SZ, SZ);
	ffly_mcd_cc();
	close(fd);
}
#endif
