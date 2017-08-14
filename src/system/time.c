# include <time.h>
# include <eint_t.h>
# include <math.h>
mdl_u64_t get_sec_tp();

mdl_u64_t ns_to_us(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.001);}

mdl_u64_t ns_to_ms(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.000001);}

mdl_u64_t ns_to_sec(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.000000001);}

mdl_u64_t ns_to_min(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.00000000001666667);}

static struct timespec t;
mdl_u64_t get_ns_tp() {
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_nsec+(t.tv_sec*1000000000);
}

mdl_u64_t get_us_tp() {return ns_to_us(get_ns_tp());}
mdl_u64_t get_ms_tp() {return ns_to_ms(get_ns_tp());}
mdl_u64_t get_sec_tp() {return ns_to_sec(get_ns_tp());}
mdl_u64_t get_min_tp() {return ns_to_min(get_ns_tp());}
