# include "max.h"
double ffly_max(double __l, double __r) {
    return __l>__r?__l:__r;
}

double ffly_max3(double __l, double __mid, double __r) {
    return ffly_max(ffly_max(__l, __mid), __r);
}
