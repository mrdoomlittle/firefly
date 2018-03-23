# include "as.h"
# include "../string.h"
# include "../malloc.h"
# include "../ffef.h"
typedef struct st {
	struct st *next;
	char const *p;
	mdl_u8_t l;
} *stp;

stp static head = NULL;
mdl_uint_t static off = 0;
mdl_uint_t stt(char const *__str, mdl_uint_t __l) {
	mdl_uint_t ret = off;
	mdl_uint_t l = (__l>0?__l:strlen(__str))+1;
	off+=l;
	
	stp p = (stp)malloc(sizeof(struct st));
	memdup((void**)&p->p, __str, l);
	p->l = l;

	p->next = head;
	head = p;
	return ret;
}

mdl_u64_t stt_drop() {
	struct ffef_reg_hdr reg;
	reg.beg = offset;
	stp cur = head;
	while(cur != NULL) {
		oust((mdl_u8_t*)cur->p, cur->l);
		free((void*)cur->p);
		stp bk = cur;
		cur = cur->next;
		free(bk);
	}
	reg.end = offset;

	mdl_u64_t ret;
	char const *name = "stt";
	reg.l = 3;
	reg.type = FF_RG_STT;
	reg.name = offset;
	oust((mdl_u8_t*)name, reg.l);

	ret = offset;
	oust((mdl_u8_t*)&reg, ffef_reg_hdrsz);
	return ret;
}
