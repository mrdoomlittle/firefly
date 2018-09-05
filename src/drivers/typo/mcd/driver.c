# include "../mcd.h"
# include "../../../typo/memalloc.h"
# include "../../../typo.h"
# include "../../../typo/raise.h"
# include "../../../tape.h"
# include "../../../stdio.h"
# include "../../../dep/mem_cpy.h"
struct typo_face *mcd_face = NULL;

void mcd_load(struct ffly_typo_sheet*, struct typo_face*);
void mcd_load_glyph(ff_u16_t);

void static
abolish(void) {

}

static struct typo_point *last_point = NULL;
static struct ffly_tape *curtape = NULL;
void static line_to(struct typo_point *__p0, struct typo_point *__p1) {
	if (!curtape) {
		printf("fatal error.\n");
		return;
	}
	ff_u8_t op;

	op = 0;
	ffly_tape_insert(curtape, &op, 1);
	ff_u16_t p0, p1;
	raise_sp-=sizeof(struct typo_point);
	p0 = raise_sp;
	ffly_mem_cpy(raise_stack+raise_sp, __p0, sizeof(struct typo_point));
	raise_sp-=sizeof(struct typo_point);
	p1 = raise_sp;
	ffly_mem_cpy(raise_stack+raise_sp, __p1, sizeof(struct typo_point));

	ffly_tape_insert(curtape, &p0, sizeof(ff_u16_t));
	ffly_tape_insert(curtape, &p1, sizeof(ff_u16_t));
}

void point(void) {
	printf("point.\n");
	if (last_point != NULL) {

	}
	struct typo_point *p;
	p = (struct typo_point*)ffly_tmalloc(sizeof(struct typo_point));

	*p = *(struct typo_point*)(raise_stack+*(ff_u16_t*)raise_p);
	if (last_point != NULL) {
		line_to(last_point, p);
	}
	last_point = p;

}

static void rn(void) {
	printf("rn.\n");
	curtape = ffly_tape_new();
}

static void rr(void) {
	printf("rr.\n");
	ffly_typo_raise(curtape);
}

static void(*op_tbl[])(void) = {
	point,
	rn,
	rr
};

static ff_u8_t sz_tbl[] = {
	sizeof(ff_u16_t),
	0,
	0
};

void ffly_typo_mcd(struct typo_driver *__driver) {
	__driver->load = mcd_load;
	__driver->load_glyph = mcd_load_glyph;
	raise_eo = op_tbl;
	raise_eo_sz = sz_tbl;
}
