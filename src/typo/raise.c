# include "raise.h"
# include "../ffly_def.h"
# include "../system/io.h"
# include "raster.h"
/*
	build

	file is only to contain exec part of things
*/
ff_u8_t *raise_p;

ff_u8_t *raise_eo_sz;
void(**raise_eo)(void);
ff_u8_t raise_stack[256];
ff_u16_t raise_sp = 256;

void static eo(void) {
	ff_u8_t on;
	on = *(raise_p++);
	raise_eo[on]();
	raise_p+=raise_eo_sz[on];
}

static void(*op[])(void) = {
    NULL,
	eo
};

ff_uint_t static os[] = {
    sizeof(ff_u16_t)*2,
	0
};

void ffly_traise_prime(void) {
	ffly_typo_raster(op);
}
# define EDGE 2

/*
	non driver control
*/
void ffly_typo_raise(struct ffly_tape *__tape) {
	ff_u8_t *end;
	ff_u8_t *rest;

	rest = raise_p;
	raise_p = (ff_u8_t*)__tape->area;
	end = raise_p+__tape->len;
	ffly_printf("tape size: %u\n", __tape->len);
	ff_u8_t on;
	ff_uint_t i;

	if (!__tape->len)
		goto _end;
	i = 0;
_again:
	ffly_printf("operation : %u, at %u\n", i++, raise_p-(ff_u8_t*)__tape->area);
	on = *(raise_p++);
	if (on>=EDGE) {
		ffly_printf("operation number illegal, got: %u\n", on);
		return;
	}

	op[on]();
	raise_p+=os[on];
	if (raise_p != end)
		goto _again;
_end:
	raise_p = rest;
}
