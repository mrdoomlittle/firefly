# include "as.h"
# include "../string.h"
# include "../malloc.h"
# include "../ffef.h"
# include "../stdio.h"
symbolp syt_head = NULL;

void ff_as_putsymbol(symbolp __sy) {
	ff_as_hash_put(&symbols, __sy->p, __sy->len, __sy);
}

ff_u16_t static off = 0;
symbolp ff_as_syt(char const *__name, ff_u16_t *__off) {
	symbolp sy = (symbolp)malloc(sizeof(struct symbol));
	sy->len = strlen(__name);

	memdup((void**)&sy->p, __name, sy->len+1);
	sy->next = syt_head;
	sy->type = FF_SY_NULL;
	syt_head = sy;
	if (__off != NULL)
		*__off = off;
	sy->off = off++;
	return sy;
}

symbolp ff_as_getsymbol(char const *__s) {
	return (symbolp)ff_as_hash_get(&symbols, __s, strlen(__s));
}

ff_u64_t syt_dst;
void(*ff_as_syt_store)(void);
void(*ff_as_syt_drop)(void);
void(*ff_as_syt_gut)(void);
