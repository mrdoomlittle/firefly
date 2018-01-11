# include "rotate.h"
# include <math.h>
# include "pi.h"
void ffly_rotate(double *__x, double *__y, float __angle) {
    double rad = __angle*(PI/180.0);
    double sn = sin(rad), cs = cos(rad);
    double x = *__x, y = *__y;
    *__x = x*cs-y*sn;
    *__y = x*sn+y*cs;
}
