# ifndef __ffly__str__len__h
# define __ffly__str__len__h
# include <mdlint.h>
# ifdef __cplusplus
extern "C" {
# endif
mdl_uint_t ffly_str_len(char*);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace data {
static uint_t(*str_len)(char*) = &ffly_str_len;
}
}
}
# endif
# endif /*__ffly__str__len__h*/
