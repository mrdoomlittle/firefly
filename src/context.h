# ifndef __ffly__context__h
# define __ffly__context__h
# include "ffint.h"
# include "types.h"
# include "driver.h"
struct ff_context {
	struct ff_driver driver;
};

static struct ff_context __ffly_grctx__;
# endif /*__ffly__context__h*/
