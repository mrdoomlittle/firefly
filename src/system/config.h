# ifndef __ffly__system__config__h
# define __ffly__system__config__h
# include "../ffint.h"
# include "../types.h"
# include "../dep/str_dup.h"

#define SCF_version			0
#define SCF_max_threads 	8
#define SCF_root_dir 		12
#define SCF_moddir			20
#define SCF_inidir			28
#define SCF_modl			36
#define SCF_inil			44
#define SCF_alssize			52
#define SCF_db_loaded		56
#define SCF_db_ip_addr		57
#define SCF_db_port			65
#define SCF_db_enckey		67
#define SCF_db_user			75
#define SCF_db_passwd		83
#define SCF_loaded			91

#define SCF_TY_version			char const*
#define SCF_TY_max_threads 		ff_u32_t
#define SCF_TY_root_dir 		char const*
#define SCF_TY_moddir			char const*
#define SCF_TY_inidir			char const*
#define SCF_TY_modl				char const**
#define SCF_TY_inil				char const**
#define SCF_TY_alssize			ff_u32_t
#define SCF_TY_db_loaded		ff_i8_t
#define SCF_TY_db_ip_addr		char const*
#define SCF_TY_db_port			ff_u16_t
#define SCF_TY_db_enckey		char const*
#define SCF_TY_db_user			char const*
#define SCF_TY_db_passwd		char const*
#define SCF_TY_loaded			ff_i8_t

#define sysconf_get(__what) \
	((SCF_TY_ ## __what*) (ffly_sysconf+(SCF_ ## __what)))
extern ff_u8_t ffly_sysconf[];
# define sysconf_loaded \
	(*sysconf_get(loaded))
# define sysconf_db_loaded \
	(*sysconf_get(db_loaded))
# ifdef __cplusplus
extern "C" {
# endif
//extern struct ffly_sysconf __ffly_sysconf__;
ff_err_t ffly_ld_sysconf(char const*);
void ffly_ld_sysconf_def(void);
void ffly_free_sysconf(void);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__system__config__h*/
