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

/*
	non driver control
*/
void ffly_typo_raise(struct typo_glyph *__glyph) {
	ff_u8_t *end;

	raise_p = (ff_u8_t*)__glyph->code;
	end = raise_p+__glyph->len;
	ff_u8_t on;
	ff_uint_t i;
	i = 0;
_again:
	ffly_printf("operation : %u\n", i++);
	on = *(raise_p++);
	if (on >0) {
		ffly_printf("operation number illegal, got: %u\n", on);
		return;
	}

	op[on]();
	raise_p+=os[on];
	if (raise_p != end)
		goto _again;
}
