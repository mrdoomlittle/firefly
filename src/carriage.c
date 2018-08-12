# include "carriage.h"
# include "ffly_def.h"
# include "memory/mem_alloc.h"
# include "memory/mem_realloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
struct carriage {
	ff_u16_t i, n;
	ff_u8_t bits;
	ff_u16_t done;

	ff_uint_t ibs;
	ff_u64_t *ib;

	ff_u16_t dud;
};


# define BUSY 0x01
static struct carriage cr[] = {
	{0, 0, 0x02, 0, 0, NULL, 0}
};

void ffly_carriage_put(ff_uint_t __c) {
	struct carriage *c = cr+__c;
	c->bits |= BUSY;
}

ff_i8_t ffly_carriage_turn(ff_uint_t __c, ff_u16_t __n) {
	struct carriage *c = cr+__c;

	ff_u8_t off, b;

	off = 63-(__n-((b = (__n>>6))*64));
	return 0-(ff_i8_t)((*(c->ib+b)>>off)&0x01);
}

ff_i8_t ffly_carriage_ready(ff_uint_t __c) {
	struct carriage *c = cr+__c;
	return -1+(ff_i8_t)(c->bits&BUSY);
}

ff_u16_t ffly_carriage_add(ff_uint_t __c) {
	struct carriage *c = cr+__c;
	ff_u16_t r;
	r = c->n++;
	if (!c->ib) {
		c->ib = (ff_u64_t*)__ffly_mem_alloc(sizeof(ff_u64_t));
		c->ibs++;
	} else {
		if (r>>6 >= c->ibs)
			c->ib = (ff_u64_t*)__ffly_mem_realloc(c->ib, (++c->ibs)*sizeof(ff_u64_t));
		else
			goto _sk;
	}
	*(c->ib+r) = 0;
_sk:

	return r;
}

void ffly_carriage_done(ff_uint_t __c, ff_u16_t __n) {
	struct carriage *c = cr+__c;

	ff_u8_t res;
	__asm__("lock incw %0" : "=m"(c->i));
_again:
	res = 0;
	__asm__("lock btrw $1, (%1)\n"
			"setcb %0"
	
	: "=m"(res) : "r"(&c->bits));

	if (!res && (c->bits&BUSY)>0) {
		goto _again;
	}

	if (res) {
		if (c->i != c->n-c->dud)
			/*
				reset
			*/
			c->bits |= 0x02;
		else
			/*
				remove busy bit
			*/
			c->bits ^= BUSY;
	}

	ff_u8_t off, b;
    off = 63-(__n-((b = (__n>>6))*63));
	__asm__("movq $1, %%rax\n"
			"movb %1, %%cl\n"
			"shlq %%cl, %%rax\n"
			"lock orq %%rax, (%0)\n"
	: : "r"(c->ib+b), "m"(off) : "rax", "rcx");
	__asm__("lock incw %0" : "=m"(c->done));
}

void ffly_carriage_dud(ff_uint_t __c) {
	cr[__c].dud++;
}

void ffly_carriage_udud(ff_uint_t __c) {
	cr[__c].dud--;
}

void ffly_carriage_wait(ff_uint_t __c) {
	struct carriage *c = cr+__c;
	if (!(c->n-c->dud))
		return;
	ffly_printf("waiting for bit to be removed.\n");
	while((c->bits&BUSY)>0) {
		if ((c->bits&BUSY)>0 && c->done == c->n-c->dud) {
			ffly_printf("error.\n");
		}
	}
	ffly_printf("waiting for all to finish.\n");
	while(c->done != c->n-c->dud);

	ff_u64_t *p;
	ff_u64_t *end;
	p = c->ib;
	end = p+c->n;
	while(p != end) {
		*p ^= *p;
		p++;
	}
}

void ffly_carriage_reset(ff_uint_t __c) {
	struct carriage *c = cr+__c;
	c->i = 0;
	c->bits = 0x02;
	c->done = 0;
}
