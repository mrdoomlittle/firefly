# ifndef __ffly__tc__h
# define __ffly__tc__h
# include "ffint.h"

struct tc_spec {
	ff_u64_t sec, nsec;
};

void ff_tc_gettime(struct tc_spec*);
# endif /*__ffly__tc__h*/
