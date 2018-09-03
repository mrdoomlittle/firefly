# include "../mcd.h"
# include "../../../typo/memalloc.h"
# include "../../../typo.h"
# include "../../../typo/raise.h"
# include "../../../typo/film.h"
# include "../../../stdio.h"
struct typo_face *mcd_face = NULL;

void mcd_load(struct ffly_typo_sheet*, struct typo_face*);
void mcd_load_glyph(ff_u16_t);

void static
abolish(void) {

}

struct typo_point *had = NULL;
struct typo_point *last_point = NULL;
struct typo_film *curfilm = NULL;
void static line_to(struct typo_point *__p0, struct typo_point *__p1) {
	if (!curfilm) {
		printf("fatal error.\n");
		return;
	}
	ff_u8_t op;

	op = 0;
	typo_film_insert(curfilm, &op, 1);
	typo_film_insert(curfilm, __p0, sizeof(struct typo_point));
	typo_film_insert(curfilm, __p1, sizeof(struct typo_point));
}

void point(void) {
	printf("point.\n");
	if (last_point != NULL) {

	}
	struct typo_point *p;
	p = (struct typo_point*)ffly_tmalloc(sizeof(struct typo_point));

	*p = *(struct typo_point*)raise_p;
	if (last_point != NULL) {
		line_to(last_point, p);
	}
	last_point = p;

}

static void rn(void) {
	printf("rn.\n");
	curfilm = typo_film_new();
}

static void rr(void) {
	printf("rr.\n");
	ffly_typo_raise(curfilm);
}

static void(*op_tbl[])(void) = {
	point,
	rn,
	rr
};

static ff_u8_t sz_tbl[] = {
	sizeof(struct typo_point),
	0,
	0
};

void ffly_typo_mcd(struct typo_driver *__driver) {
	__driver->load = mcd_load;
	__driver->load_glyph = mcd_load_glyph;
	raise_eo = op_tbl;
	raise_eo_sz = sz_tbl;
//	__driver->abolish = abolish;
}
