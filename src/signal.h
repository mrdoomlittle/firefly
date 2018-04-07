# ifndef __ffly__signal__h_
# define __ffly__signal__h_
# include "linux/signal.h"
# include <mdlint.h>
mdl_s32_t sigaction(mdl_s32_t, struct sigaction const*, struct sigaction*);
# endif /*__ffly__signal__h_*/
