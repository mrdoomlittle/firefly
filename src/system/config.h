# ifndef __ffly__system__config__h
# define __ffly__system__config__h
# include "../ffint.h"
# include "../types.h"
# include "../dep/str_dup.h"
struct ffly_sysconf {
    char const *version;
    ff_uint_t max_threads;
	char const *root_dir;
	char const *moddir, *inidir;
	char const **modl, **inil;
	ff_uint_t alssize;
};

# ifdef __cplusplus
extern "C" {
# endif
extern struct ffly_sysconf __ffly_sysconf__;
ff_err_t ffly_ld_sysconf(char const*);
void ffly_ld_sysconf_def();
void ffly_free_sysconf();
# ifdef __cplusplus
}
# endif
# endif /*__ffly__system__config__h*/
