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
    sizeof(struct typo_point)*2,
	0
};

void ffly_traise_prime(void) {
	ffly_typo_raster(op);
}
# define EDGE 2

/*
	non driver control
*/
void ffly_typo_raise(struct typo_film *__film) {
	ff_u8_t *end;
	ff_u8_t *rest;

	rest = raise_p;
	raise_p = (ff_u8_t*)__film->area;
	end = raise_p+__film->len;
	ffly_printf("film size: %u\n", __film->len);
	ff_u8_t on;
	ff_uint_t i;

	if (!__film->len)
		goto _end;
	i = 0;
_again:
	ffly_printf("operation : %u\n", i++);
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
