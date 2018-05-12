# include "as.h"
# include "../string.h"
# include "../malloc.h"
# include "../ffef.h"
typedef struct st {
	struct st *next;
	char const *p;
	ff_u8_t l;
} *stp;

stp static head = NULL;
ff_uint_t static off = 0;
ff_uint_t ff_as_stt(char const *__str, ff_uint_t __l) {
	ff_uint_t l = (__l>0?__l:strlen(__str))+1;
	off+=l;
	
	stp p = (stp)malloc(sizeof(struct st));
	memdup((void**)&p->p, __str, l);
	p->l = l;

	p->next = head;
	head = p;
	return off;
}

ff_u64_t ff_as_stt_drop() {
	struct ffef_reg_hdr reg;
	reg.beg = offset;
	stp cur = head;
	while(cur != NULL) {
		ff_as_oust((ff_u8_t*)cur->p, cur->l);
		free((void*)cur->p);
		stp bk = cur;
		cur = cur->next;
		free(bk);
	}
	reg.end = offset;

	ff_u64_t ret;
	char const *name = "stt";
	reg.l = 3;
	reg.type = FF_RG_STT;
	reg.name = offset;
	ff_as_oust((ff_u8_t*)name, reg.l);

	ret = offset;
	ff_as_oust((ff_u8_t*)&reg, ffef_reg_hdrsz);
	return ret;
}
