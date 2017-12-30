# ifndef __ffly__http__h
# define __ffly__http__h
# include "../types/err_t.h"
# include "../types/size_t.h"
# include <mdlint.h>
ffly_err_t ffly_http_get(char*, char*, mdl_u16_t, void**, ffly_size_t*);
# endif /*__ffly__http__h*/
