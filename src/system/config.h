# ifndef __ffly__system__config__h
# define __ffly__system__config__h
# include "../types/err_t.h"
# include "../data/str_dupe.h"
struct ffly_sysconf {
    char const *version;
    mdl_uint_t max_threads;
};

# ifdef __cplusplus
extern "C" {
# endif
extern struct ffly_sysconf __ffly_sysconf__;
ffly_err_t ffly_ld_sysconf(char*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__system__config__h*/
