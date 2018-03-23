# include "as.h"
# include "../string.h"
# include "../malloc.h"
# include "../ffef.h"
# include "../stdio.h"
symbolp static head = NULL;

void putsymbol(symbolp __sy) {
	hash_put(&symbols, __sy->p, __sy->len, __sy);
}

symbolp syt(char const *__name) {
	symbolp sy = (symbolp)malloc(sizeof(struct symbol));
	sy->len = strlen(__name);

	memdup((void**)&sy->p, __name, sy->len+1);
	sy->next = head;
	head = sy;
	return sy;
}

symbolp getsymbol(char const *__s) {
	return (symbolp)hash_get(&symbols, __s, strlen(__s));
}

void syt_drop() {
	regionp reg = (regionp)_alloca(sizeof(struct region));
	reg->beg = offset;
	symbolp cur = head;
	while(cur != NULL) {
		struct ffef_sy sy;
		sy.name = stt(cur->p, cur->len);
		/*
		if (is_sylabel(cur)) {
			labelp la = (labelp)hash_get(&env, cur->p, cur->len);


			sy.loc = la->offset;
		}
*/
		sy.l = cur->len+1;
		oust((mdl_u8_t*)&sy, ffef_sysz);
		symbolp bk = cur;
		cur = cur->next;

		free(bk->p);
		free(bk);
	}

	reg->end = offset;
	strcpy(reg->name = _alloca(4), "syt");
	reg->next = curreg;
	curreg = reg;
}
