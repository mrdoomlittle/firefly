# ifndef __ffly__system__config__h
# define __ffly__system__config__h
# include "../types/err_t.h"
# include "../dep/str_dup.h"
struct ffly_sysconf {
    char const *version;
    mdl_uint_t max_threads;
	char const *root_dir;
};

# ifdef __cplusplus
extern "C" {
# endif
extern struct ffly_sysconf __ffly_sysconf__;
ffly_err_t ffly_ld_sysconf(char const*);
void ffly_free_sysconf();
# ifdef __cplusplus
}
# endif
# endif /*__ffly__system__config__h*/
