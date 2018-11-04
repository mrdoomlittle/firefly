# include "tc.h"
# include "stdio.h"
# include "maths.h"
void ff_time_init(void) {
	struct tc_spec dummy;
	ff_tc_gettime(&dummy);

	float v;
	v = dummy.sec;
	float d, h, m, s;
	d = v/86400.;
	v-=ffly_floor(d)*86400.;
	h = v/3600.;
	v-=ffly_floor(h)*3600.;
	m = v/60.;
	v-=ffly_floor(m)*60;
	s = v;

	printf("current time: %u-%u-%u-%u or %u\n", (ff_u64_t)d, (ff_u64_t)h, (ff_u64_t)m, (ff_u64_t)s, dummy.sec);
}
