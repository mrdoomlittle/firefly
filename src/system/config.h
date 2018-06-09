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
	struct {
		ff_i8_t loaded;
		char const *ip_addr;
		ff_u16_t port;
		char const *enckey;
		char const *user;
		char const *passwd;
	} db;
	ff_i8_t loaded;
};

# define sysconf_loaded \
	__ffly_sysconf__.loaded
# define sysconf_db_loaded \
	__ffly_sysconf__.db.loaded
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
