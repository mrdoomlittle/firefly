# include "typo.h"
# include "drivers/typo.h"
# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "dep/mem_set.h"
# include "typo/raster.h"
# include "typo/raise.h"
# include "system/io.h"
# include "typo/memalloc.h"
void(*driver_load[])(struct typo_driver*) = {
	ffly_typo_mcd
};

static struct typo_driver driver;
void(*typo_read)(void*, ff_uint_t);
void(*typo_write)(void*, ff_uint_t);
void(*typo_seek)(ff_u64_t, ff_u32_t);

int static fd;
void static _seek(ff_u64_t __off, ff_u32_t __whence) {
	lseek(fd, __off, __whence);
}

void static _read(void *__buf, ff_uint_t __size) {
	read(fd, __buf, __size);
}

void static _write(void *__buf, ff_uint_t __size) {
	write(fd, __buf, __size);
}

struct typo_glyph **glyph_table;

struct typo_glyph*
ffly_typo_glyph_new(ff_u16_t __idx) {
	struct typo_glyph *g;

	g = (struct typo_glyph*)ffly_tmalloc(sizeof(struct typo_glyph));
	*(glyph_table+__idx) = g;

	return g;
}

void ffly_typo_init(ffly_typo_sheetp __sheet, ff_u8_t __driver) {
	driver_load[__driver](&driver);
	typo_read = _read;
	typo_write = _write;
	typo_seek = _seek;
	glyph_table = (struct typo_glyph**)ffly_tmalloc(0x100);
	ffly_mem_set(glyph_table, 0, 0x100*sizeof(struct typo_glyph*));
}

void ffly_typo_face(ffly_typo_sheetp __sheet, char const *__path, struct typo_face *__face) {
	fd = open(__path, O_RDONLY, 0);
	driver.load(__sheet, __face);
	close(fd);
}

void ffly_typo_char_load(ffly_typo_sheetp __sheet, struct typo_face *__face, char __c) {
	driver.load_glyph(__c);
	// build it
	ffly_typo_raise(__face->glyph->tape);
}

void ffly_typo_face_scale(ffly_typo_sheetp __sheet, struct typo_face *__face, ff_u8_t __scale) {
	ff_uint_t size;

	size = (typo_raster_width = (__sheet->f_width<<__scale))
		*(typo_raster_height = (__sheet->f_height<<__scale));
	if (!typo_raster_bm) {
		typo_raster_bm = (ff_u8_t*)ffly_tmalloc(size);
		goto _sk;
	}

	typo_raster_bm = (ff_u8_t*)ffly_trealloc(typo_raster_bm, size);
_sk:
	ffly_mem_set(typo_raster_bm, 0, size);
	typo_raster_scale = __scale;
}

void ffly_typo_done(ffly_typo_sheetp __sheet) {
	ffly_tmpurge();
}

void static dbm(void) {
	ff_uint_t x, y;
	ffly_printf("\n");
	y = 0;
	while(y != typo_raster_height) {
		x = 0;
		while(x != typo_raster_width) {
			if (!x || !y || x == typo_raster_width-1 || y == typo_raster_height-1) {
				ffly_printf("+");
			} else
				ffly_printf("%c", !typo_raster_bm[x+(y*typo_raster_width)]?' ':'#');
			x++;
		}
		ffly_printf("\n");
		y++;
	}
}
//# define DEBUG
# ifdef DEBUG

# include "system/string.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	struct ffly_typo_sheet sheet;
	ffly_typo_init(&sheet, 0);
	ffly_traise_prime();
	ff_uint_t scale;
	if (__argc>1)
		scale = ffly_stno(__argv[1]);
	else
		scale = 0;
	struct typo_face face;
	ffly_typo_face(&sheet, "tools/test.mcd", &face);
	ffly_typo_face_scale(&sheet, &face, scale);
	ffly_typo_char_load(&sheet, &face, 0);
	dbm();
	ffly_typo_done(&sheet);
}
# endif
