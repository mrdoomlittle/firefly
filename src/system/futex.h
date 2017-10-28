# ifndef __ffly__futex__h
# define __ffly__futex__h
# ifdef __cplusplus
extern "C" {
# endif
void _flock();
void _funlock();
void flock(int*, int*);
void funlock(int*, int*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__futex__h*/
