# include "min.h"
double ffly_min(double __l, double __r) {
    return __l<__r?__l:__r;
}

double ffly_min3(double __l, double __mid, double __r) {
    return ffly_min(ffly_min(__l, __mid), __r);
}
