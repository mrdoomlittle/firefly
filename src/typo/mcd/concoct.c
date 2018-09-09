# include "concoct.h"
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
	return (void*)((((ff_u8_t*)__vec->p)+((__vec->n-1)*__vec->bsz)));
}

static struct header cc_header;
static struct vec cc_segs;
static struct vec cc_glyphs;

static ff_u32_t off =
	sizeof(struct header);

void*
ffly_mcd_cc_seg_alloc(struct cc_segment *__s, ff_uint_t __size) {
	__s->sz+=__size;
	if (!__s->p)
		__s->p = __ffly_mem_alloc(__s->sz);
	else
		__s->p = __ffly_mem_realloc(__s->p, __s->sz);
	return (void*)(((ff_u8_t*)__s->p)+(__s->sz-__size));
}

struct cc_segment*
ffly_mcd_cc_seg_new(void) {
	struct cc_segment **s;
	s = (struct cc_segment**)vec_new(&cc_segs);
	struct cc_segment *p;
	p = (struct cc_segment*)__ffly_mem_alloc(sizeof(struct cc_segment));
	p->p = NULL;
	p->sz = 0;
	*s = p;
	return p;
}

struct cc_glyph*
ffly_mcd_cc_glyph_new(void) {
	struct cc_glyph **g;
	g = (struct cc_glyph**)vec_new(&cc_glyphs);
	struct cc_glyph *p;
	p = (struct cc_glyph*)__ffly_mem_alloc(sizeof(struct cc_glyph));
	p->tape = NULL;
	*g = p;
	return p;
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
		ffly_printf("glyph has tape.\n");
		cc_write(__g->tape->area, __g->tape->len, off);	
		__g->text.code = off;
		off+=__g->tape->len;
		__g->text.len = __g->tape->len;
	} else {
		__g->text.code = 0;
		__g->text.len = 0;
	}
	cc_write(&__g->text, sizeof(struct glyph), dst+(idx*sizeof(struct glyph)));
	ffly_printf("glyph: code: %u, len: %u, idx: %u\n", __g->text.code, __g->text.len, __g->text.idx);
}

void ffly_mcd_cc_prime(void) {
	vec_init(cc_segs, sizeof(struct cc_segment*));
	vec_init(cc_glyphs, sizeof(struct cc_glyph*));
}

ff_uint_t ffly_mcd_cc_face_width, ffly_mcd_cc_face_height;

void ffly_mcd_cc(void) {
	struct cc_segment **s, **se;
	s = (struct cc_segment**)vec_front(cc_segs);
	se = (struct cc_segment**)vec_end(cc_segs);

	cc_header.seg = off;
	dst = off;
	idx = 0;
	off+=cc_segs.n*sizeof(struct segment);
	while(s != se) {
		seg_out(*s);
		s++;
	}

	struct cc_glyph **g, **ge;
	g = (struct cc_glyph**)vec_front(cc_glyphs);
	ge = (struct cc_glyph**)vec_end(cc_glyphs);

	cc_header.glyph_table = off;
	cc_header.glyph_c = 0;
	dst = off;
	idx = 0;
	off+=cc_glyphs.n*sizeof(struct glyph);
	while(g != ge) {
		(*g)->text.idx = idx;
		glyph_out(*g);
		cc_header.glyph_c++;
		idx++;
		g++;
	}

	cc_header.width = ffly_mcd_cc_face_width;
	cc_header.height = ffly_mcd_cc_face_height;
	ffly_printf("header: width: %u, height: %u\nglyph table: %u, glyph count: %u, segments: %u\n", cc_header.width, cc_header.height,
		cc_header.glyph_table, cc_header.glyph_c, cc_header.seg);
	cc_write(&cc_header, sizeof(struct header), 0);
}
