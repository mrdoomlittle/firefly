# include "round.h"
double extern _ffly_round(double);
double ffly_round(double __val) {
    double ret_val;
    __asm__("movsd %1, %%xmm0\n\t"
        "call _ffly_round\n\t"
        "movsd %%xmm0, %0" : "=m"(ret_val) : "m"(__val));
    return ret_val;
}
