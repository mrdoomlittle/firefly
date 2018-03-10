# include "copy.h"
# include "ring.h"
# include "../call.h"
# include "../ffly_def.h"
void dcopy(void *__dst, void *__src, mdl_uint_t __n) {
	mdl_u8_t bed[16+sizeof(mdl_uint_t)];
	mdl_u8_t *p = bed;
	*(void**)p = __dst;
	p+=8;
	*(void**)p = __src;
	p+=8;
	*(mdl_uint_t*)p = __n;
	ffmod_ring(_ffcal_mod_dcp, NULL, bed);
}

void scopy(void *__dst, void *__src, mdl_uint_t __n) {
	mdl_u8_t bed[16+sizeof(mdl_uint_t)];
	mdl_u8_t *p = bed;
	*(void**)p = __dst;
	p+=8;
	*(void**)p = __src;
	p+=8;
	*(mdl_uint_t*)p = __n;
	ffmod_ring(_ffcal_mod_scp, NULL, bed);
}
