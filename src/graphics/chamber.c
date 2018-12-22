# include "chamber.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../log.h"
struct object {
	void(*func)(long long);
	long long arg;
	struct object *next, **bk;
	ff_uint_t n;
};

struct chamber {
	struct object *top, *uu;
	struct object **objs;
	ff_uint_t obc;
};

static struct chamber *ch = NULL;
ff_u32_t static off = 0;
ff_u32_t ffly_cch;

void ffly_chamber_run(ff_u32_t __ch) {
	_ff_log("running chamber: %u\n", (ch+__ch)->obc);
	struct object *co;
	co = (ch+__ch)->top;
	while(co != NULL) {
		co->func(co->arg);
		co = co->next;
	}
}
#define deattach(__ob)\
	*__ob->bk = __ob->next;\
	if (__ob->next != NULL)\
		__ob->next->bk = __ob->bk;

ff_u32_t ffly_chamber_add(void(*__func)(long long), long long __arg, ff_u32_t __ch) {
	_ff_log("adding to chamber.\n");
	struct chamber *c;
	c = ch+__ch;
	
	struct object *ob;
	ff_uint_t rt;
	if (c->uu != NULL) {
		ob = c->uu;
		c->uu = ob->next;
		c->uu->bk = &c->uu;
		rt = ob->n;
		goto _sk;
	}

	ob = (struct object*)__ffly_mem_alloc(sizeof(struct object));	
	if (c->top != NULL)
		c->top->bk = &ob->next;
	ob->bk = &c->top;
	ob->next = c->top;
	c->top = ob;

	if (!c->objs) {
		c->objs = (struct object**)__ffly_mem_alloc(sizeof(struct object*));
		c->obc++;
	} else
		c->objs = (struct object**)__ffly_mem_realloc(c->objs, (++c->obc)*sizeof(struct object*));
	c->objs[rt = (c->obc-1)] = ob;

	ob->n = rt;
_sk:
	ob->func = __func;
	ob->arg = __arg;
	return rt;
}

void ffly_chamber_rm(ff_u32_t __ch, ff_uint_t __n) {
	_ff_log("removing from chamber.\n");
	struct chamber *c;
	c = ch+__ch;

	struct object *ob;
	ob = c->objs[__n];

	deattach(ob);
	ob->bk = &c->uu;
	ob->next = c->uu;
	if (c->uu != NULL)
		c->uu->bk = &ob->next;
	c->uu = ob;
}

ff_u32_t ffly_chamber_new(void) {
	ff_u32_t rv; 
	rv = off++;

	if (!ch) {
		ch = (struct chamber*)__ffly_mem_alloc(sizeof(struct chamber));
	} else {
		ch = (struct chamber*)__ffly_mem_realloc(ch, off*sizeof(struct chamber));
	}

	struct chamber *c;
	c = ch+rv;

	c->objs = NULL;
	c->obc = 0;
	c->top = NULL;
	c->uu = NULL;
	return rv;
}
