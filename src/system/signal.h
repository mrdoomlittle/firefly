# ifndef __ffly__signal__h
# define __ffly__signal__h
# include <mdlint.h>
typedef mdl_u16_t ffly_sig_t;

# ifdef __cplusplus
extern "C" {
# endif
void extern ffly_sigwait(ffly_sig_t*);
void extern ffly_signal(ffly_sig_t*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__signal__h*/
