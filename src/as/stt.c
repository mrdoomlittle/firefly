# include "as.h"
# include "../string.h"
# include "../malloc.h"
# include "../ffef.h"
stp stt_head = NULL;
stp stt_tail = NULL;
ff_uint_t stt_off = 0;
ff_uint_t ff_as_stt(char const *__str, ff_uint_t __l) {
	ff_uint_t l = (__l>0?__l:strlen(__str))+1;
	stt_off+=l;
	
	stp p = (stp)malloc(sizeof(struct st));
	memdup((void**)&p->p, __str, l);
	p->l = l;
	if (!stt_tail)
		stt_tail = p;
	if (stt_head != NULL)
		stt_head->prev = p;
	p->prev = NULL;
	p->next = stt_head;
	stt_head = p;
	return stt_off;
}

ff_u64_t(*ff_as_stt_drop)(void);
