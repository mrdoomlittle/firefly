# include <time.h>
# include <mdlint.h>
# include <math.h>
mdl_u64_t ffly_get_sec_tp();
mdl_u64_t ffly_ns_to_us(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.001);}

mdl_u64_t ffly_ns_to_ms(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.000001);}

mdl_u64_t ffly_ns_to_sec(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.000000001);}

mdl_u64_t ffly_ns_to_min(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.00000000001666667);}

static struct timespec t;
mdl_u64_t ffly_get_ns_tp() {
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_nsec+(t.tv_sec*1000000000);
}

mdl_u64_t ffly_get_us_tp() {return ffly_ns_to_us(ffly_get_ns_tp());}
mdl_u64_t ffly_get_ms_tp() {return ffly_ns_to_ms(ffly_get_ns_tp());}
mdl_u64_t ffly_get_sec_tp() {return ffly_ns_to_sec(ffly_get_ns_tp());}
mdl_u64_t ffly_get_min_tp() {return ffly_ns_to_min(ffly_get_ns_tp());}
