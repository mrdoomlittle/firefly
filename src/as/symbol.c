# include "as.h"
# include "../string.h"
# include "../malloc.h"
# include "../ffef.h"
# include "../stdio.h"
symbolp static head = NULL;

void ff_as_putsymbol(symbolp __sy) {
	ff_as_hash_put(&symbols, __sy->p, __sy->len, __sy);
}

ff_u16_t static off = 0;
symbolp ff_as_syt(char const *__name, ff_u16_t *__off) {
	symbolp sy = (symbolp)malloc(sizeof(struct symbol));
	sy->len = strlen(__name);

	memdup((void**)&sy->p, __name, sy->len+1);
	sy->next = head;
	sy->type = FF_SY_NULL;
	head = sy;
	if (__off != NULL)
		*__off = off;
	sy->off = off++;
	return sy;
}

symbolp ff_as_getsymbol(char const *__s) {
	return (symbolp)ff_as_hash_get(&symbols, __s, strlen(__s));
}

struct ffef_reg_hdr static reg;
ff_u64_t static dst;
void ff_as_syt_store(void) {
	reg.name = dst;
	reg.l = 4;
	reg.type = FF_RG_SYT;
	lseek(out, dst, SEEK_SET);
	write(out,"syt", 4);
	write(out, &reg, ffef_reg_hdrsz);
}

void ff_as_syt_drop(void) {
	dst = offset;
	offset+=4+ffef_reg_hdrsz;
}

void ff_as_syt_gut(void) {
	reg.beg = offset;
	symbolp cur = head;
	while(cur != NULL) {
		struct ffef_sy sy;
		sy.name = ff_as_stt(cur->p, cur->len);
		sy.type = cur->type;
		sy.reg = 0;
		if (is_sylabel(cur)) {
			labelp la = (labelp)ff_as_hash_get(&env, cur->p, cur->len);
			sy.reg = la->reg->no;  
			sy.loc = la->adr;
		}

		sy.l = cur->len+1;
		ff_as_oust((ff_u8_t*)&sy, ffef_sysz);
		symbolp bk = cur;
		cur = cur->next;

		free(bk->p);
		free(bk);
	}
	reg.end = offset;
}
