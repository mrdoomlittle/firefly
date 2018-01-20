# ifndef __ffly__mem_cpy__h
# define __ffly__mem_cpy__h
# include <mdlint.h>
# include "../types/err_t.h"
# ifdef __cplusplus
extern "C" {
# endif
void _ffly_mem_cpy(void*, void*, mdl_uint_t);
ffly_err_t ffly_mem_cpy(void*, void*, mdl_uint_t);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace data {
static ffly_err_t(*mem_cpy)(void*, void*, mdl_uint_t) = &ffly_mem_cpy;
}
}
}
# endif
# endif /*__ffly__mem_py__h*/
