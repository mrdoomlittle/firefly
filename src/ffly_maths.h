# ifndef __ffly__maths__h
# define __ffly__maths__h
double extern ffly_round(double);
double extern ffly_ceil(double);
double extern ffly_floor(double);
double extern ffly_sq(double);
# ifdef __cplusplus
namespace mdl {
namespace firefly {
double __inline__ round(double __val) {
	return ffly_round(__val);}

double __inline__ ceil(double __val) {
	return ffly_ceil(__val);}

double __inline__ floor(double __val) {
	return ffly_floor(__val);}

double __inline__ sq(double __val) {
	return ffly_sq(__val);}
}
}
# endif
# endif /*__ffly__maths__h*/
