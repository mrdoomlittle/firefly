# include "slagprog.h"
# include "raise.h"
# include "memalloc.h"
# include "../system/io.h"
# include "../dep/mem_cpy.h"
static ff_u8_t *cc;
struct slgio slg;
void static _ld(void) {

}

void static _st(void) {

}
void static _test(void) {
	float r, g, b, a;
	ff_u8_t *px;
	px = (ff_u8_t*)&slg.px;
	r = *px;
	g = px[1];
	b = px[2];
	a = px[3];
	float z;
	z = slg.z;

//	ffly_printf("%u\n", (ff_u32_t)z);
	r = r-(z*(r/2000.));
	g = g-(z*(g/2000.));
	b = b-(z*(b/2000.));

//	a = (1-(1./z))*a;

	ff_u32_t out;
	out = ((ff_u8_t)r)|((ff_u8_t)g)<<8|((ff_u8_t)b)<<16|((ff_u8_t)a)<<24;
	slg.px = out;
}

void static(*op[])(void) = {
	_st,
	_ld,
	_test
};

ff_uint_t static ops[] = {
	0,
	0,
	0
};

struct slagprog {
	ff_u8_t *p;
	ff_u32_t size;
};

ff_uint_t static nprogs = 0;
static struct slagprog progs[20];

void nt_slg_run(ff_u8_t*, ff_uint_t);
void nt_slags_exec(void) {
	ff_uint_t i;
	i = 0;
	struct slagprog *slg;
	while(i != nprogs) {
		slg = progs+(i++);
		nt_slg_run(slg->p, slg->size);
	}
}

void nt_slg_prog(void) {
	ff_u8_t *p;
	ff_u32_t size;

	p = *(ff_u8_t**)nt_raise_p;
	size = *(ff_u32_t*)(nt_raise_p+8);

	struct slagprog *slg;
	slg = progs+nprogs;
	slg->p = nt_mem_alloc(size);
	slg->size = size;
	ffly_mem_cpy(slg->p, p, size);
	nprogs++;
}

void nt_slg_run(ff_u8_t *__start, ff_uint_t __si) {
	ff_u8_t on;
	ff_u8_t *end;
	cc = __start;
	end = cc+__si;
	while(cc != end) {
		on = *(cc++);
		op[on]();
		cc+=ops[on];
	}
}
