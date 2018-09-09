# include "concoct.h"
# include "struc.h"
# include "../../memory/mem_alloc.h"
# include "../../memory/mem_realloc.h"
# include "../../memory/mem_free.h"
# include "../../tape.h"
# include "../../system/io.h"
// prepare
struct vec {
	void *p;
	ff_uint_t n, bsz;
};

# define vec_init(__v, __bsz) \
	__v.p = NULL;	\
	__v.n = 0;		\
	__v.bsz = __bsz;

# define vec_front(__v) \
	((__v).p)
# define vec_end(__v) \
	((void*)(((ff_u8_t*)(__v).p)+((__v).n*(__v).bsz)))
void static* vec_new(struct vec *__vec) {
	__vec->n++;
	if (!__vec->p)
		__vec->p = __ffly_mem_alloc(__vec->n*__vec->bsz);
	else
		__vec->p = __ffly_mem_realloc(__vec->p, __vec->n*__vec->bsz);
	return (void*)((((ff_u8_t*)__vec->p)+(__vec->n*__vec->bsz))-__vec->bsz);
}

struct cc_segment {
	struct segment text;
	ff_uint_t sz;
	void *p;
};

struct cc_glyph {
	struct glyph text;
	struct ffly_tape *tape;
};

static struct header cc_header;
static struct vec cc_segs;
static struct vec cc_glyphs;

static ff_u32_t off =
	sizeof(struct header);

# define seg_off(__p, __s) \
	(((ff_u8_t*)(__p))-((ff_u8_t*)(__s)->p))

void static*
seg_alloc(struct cc_segment *__s, ff_uint_t __size) {
	__s->sz+=__size;
	if (!__s->p)
		__s->p = __ffly_mem_alloc(__s->sz);
	else
		__s->p = __ffly_mem_realloc(__s->p, __s->sz);
	return (void*)(((ff_u8_t*)__s->p)+(__s->sz-__size));
}

static struct cc_segment*
seg_new(void) {
	struct cc_segment *s;
	s = (struct cc_segment*)vec_new(&cc_segs);
	s->p = NULL;
	s->sz = 0;
	return s;
}

static struct cc_glyph*
glyph_new(void) {
	struct cc_glyph *g;
	g = (struct cc_glyph*)vec_new(&cc_glyphs);
	g->tape = NULL;
	return g;
}

void(*ffly_mcd_cc_write)(void*, ff_uint_t, ff_u32_t);

# define cc_write ffly_mcd_cc_write

ff_u32_t static dst;
ff_uint_t static idx;

void static
seg_out(struct cc_segment *__s) {
	__s->text.src = off;
	__s->text.size = __s->sz;
	cc_write(__s->p, __s->sz, off);
	off+=__s->sz;

	cc_write(&__s->text, sizeof(struct segment), dst+(idx*sizeof(struct segment)));
	ffly_printf("segment: src: %u, dst: %u, size: %u\n", __s->text.src, __s->text.dst, __s->text.size);
}


void static
glyph_out(struct cc_glyph *__g) {
	if (__g->tape != NULL) {
		cc_write(__g->tape->area, __g->tape->len, off);	
		__g->text.code = off;
		off+=__g->tape->len;
		__g->text.len = __g->tape->len;
	}
	cc_write(&__g->text, sizeof(struct glyph), dst+(idx*sizeof(struct glyph)));
	ffly_printf("glyph: code: %u, len: %u, idx: %u\n", __g->text.code, __g->text.len, __g->text.idx);
}

void ffly_mcd_cc_prime(void) {
	vec_init(cc_segs, sizeof(struct cc_segment));
	vec_init(cc_glyphs, sizeof(struct cc_glyph));
}

static ff_uint_t face_width, face_height;

void ffly_mcd_cc(void) {
	struct cc_segment *s, *se;
	s = (struct cc_segment*)vec_front(cc_segs);
	se = (struct cc_segment*)vec_end(cc_segs);

	cc_header.seg = off;
	dst = off;
	idx = 0;
	off+=cc_segs.n*sizeof(struct segment);
	while(s != se) {
		seg_out(s);
		s++;
	}

	struct cc_glyph *g, *ge;
	g = (struct cc_glyph*)vec_front(cc_glyphs);
	ge = (struct cc_glyph*)vec_end(cc_glyphs);

	
	cc_header.glyph_table = off;
	cc_header.glyph_c = 0;
	dst = off;
	idx = 0;
	off+=cc_glyphs.n*sizeof(struct glyph);
	while(g != ge) {
		g->text.idx = cc_header.glyph_c;
		glyph_out(g);
		cc_header.glyph_c++;
		g++;
	}

	cc_header.width = face_width;
	cc_header.height = face_height;
	ffly_printf("header: width: %u, height: %u\nglyph table: %u, glyph count: %u, segments: %u\n", cc_header.width, cc_header.height,
		cc_header.glyph_table, cc_header.glyph_c, cc_header.seg);
	cc_write(&cc_header, sizeof(struct header), 0);
}

# include "../../typo.h"
# define N 9
static struct typo_point points[] = {
	{29, 29},
	{16, 2},
	{2, 29},
	{6, 29},
	{10, 24}, // mid inner
	{16, 8},
	{20, 20}, // mid inner
	{25, 29},
	{29, 29}
};

# include "../../dep/mem_cpy.h"
# include "../../linux/unistd.h"
# include "../../linux/stat.h"
# include "../../linux/fcntl.h"
static int fd;
void static _write(void *__buf, ff_uint_t __size, ff_u32_t __offset) {
	pwrite(fd, __buf, __size, __offset);
}

ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_mcd_cc_write = _write;
	ffly_mcd_cc_prime();
	struct cc_glyph *g;
	g = glyph_new();
	struct cc_segment *s;
	s = seg_new();
	s->text.dst = 0;
	struct typo_point *p;
	p = (struct typo_point*)seg_alloc(s, N*sizeof(struct typo_point));
	ffly_mem_cpy(p, points, N*sizeof(struct typo_point));

	g->tape = ffly_tape_new();
	ff_u8_t code[] = {
		0x01,
		0x01,
		0x02,
		0x00, 0x00, // number of points
		0x00, 0x00, // address of points within stack 
		0x01,
		0x02
	};

	*(ff_u16_t*)(code+3) = N;
	*(ff_u16_t*)(code+5) = seg_off(p, s);
	
	ffly_tape_insert(g->tape, code, sizeof(code));
	fd = open ("test.mcd", O_RDWR|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);

	face_width = 32;
	face_height = 32;
	ffly_mcd_cc();
	close(fd);
}
