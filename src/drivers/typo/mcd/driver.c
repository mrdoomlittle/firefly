# include "../mcd.h"
# include "../../../typo.h"
# include "../../../typo/raise.h"
struct typo_face *mcd_face = NULL;

void mcd_load(struct ffly_typo_sheet*, struct typo_face*);
void mcd_load_glyph(ff_u16_t);

void static
abolish(void) {

}


static void(*op_tbl[])(void) = {


};

static ff_u8_t sz_tbl[] = {

};

void ffly_typo_mcd(struct typo_driver *__driver) {
	__driver->load = mcd_load;
	__driver->load_glyph = mcd_load_glyph;
	raise_eo = op_tbl;
	raise_eo_sz = sz_tbl;
//	__driver->abolish = abolish;
}
