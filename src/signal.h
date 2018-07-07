# ifndef __ffly__signal__h_
# define __ffly__signal__h_
# include "linux/signal.h"
# include "ffint.h"
void sigemptyset(sigset_t*);
ff_s32_t sigaction(ff_s32_t, struct sigaction const*, struct sigaction*);
# endif /*__ffly__signal__h_*/
