# ifndef __ffly__str__dupe__h
# define __ffly__str__dupe__h
# ifdef __cplusplus
extern "C" {
# endif
char* ffly_str_dupe(char const*);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace data {
static char*(*str_dupe)(char const*) = &ffly_str_dupe;
}
}
}
# endif
# endif /*__ffly__str__dupe__h*/
