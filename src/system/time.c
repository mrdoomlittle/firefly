# include <time.h>
# include <mdlint.h>
# include <math.h>
# include "time.h"
mdl_u64_t ffly_get_sec();
mdl_u64_t ffly_ns_to_us(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.001);}

mdl_u64_t ffly_ns_to_ms(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.000001);}

mdl_u64_t ffly_ns_to_sec(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.000000001);}

mdl_u64_t ffly_ns_to_min(mdl_u64_t __ns) {
	return (mdl_u64_t)round((double)__ns*0.00000000001666667);}


void ffly_gettime(ffly_timespec *__time){
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    __time->sec = time.tv_sec;
    __time->nsec = time.tv_nsec;
}

mdl_u64_t ffly_get_ns() {
    ffly_timespec time;
    ffly_gettime(&time);
    return time.nsec+(time.sec*1000000000);
}
mdl_u64_t ffly_get_us() {
    return ffly_ns_to_us(ffly_get_ns());
}
mdl_u64_t ffly_get_ms() {
    return ffly_ns_to_ms(ffly_get_ns());
}
mdl_u64_t ffly_get_sec() {
    ffly_timespec time;
    ffly_gettime(&time);
    return time.sec;
}
mdl_u64_t ffly_get_min() {
    return ffly_get_sec()/60;   
}
