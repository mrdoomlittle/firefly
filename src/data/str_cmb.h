# ifndef __ffly__str__cmb__h
# define __ffly__str__cmb__h
# include <mdlint.h>
# define _ffly_stc_free_both 0x1
# define _ffly_stc_free_lhs 0x2
# define _ffly_stc_free_rhs 0x3
# ifdef __cplusplus
extern "C" {
# endif
char* ffly_str_cmb(char*, char*, mdl_u8_t);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace data {
static char*(*str_cmb)(char*, char*, mdl_u8_t) = &ffly_str_cmb;
}
}
}
# endif
# endif /*__ffly__str__cmb__h*/
