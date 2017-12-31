# include "floor.h"
double extern _ffly_floor(double);
double ffly_floor(double __val) {
    double ret_val;
    __asm__("movsd %1, %%xmm0\n\t"
        "call _ffly_floor\n\t"
        "movsd %%xmm0, %0" : "=m"(ret_val) : "m"(__val));
    return ret_val;
}
