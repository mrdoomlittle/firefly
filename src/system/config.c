# include "config.h"
# include "../bole.h"
# include "errno.h"
# include "err.h"
# include "io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../mode.h"
# include "../dep/mem_set.h"
# define DEF_MAX_THREADS 20
# define ALLOCA_SSIZE 400

ff_u8_t ffly_sysconf[256];
#define _MAX_THREADS		0x0000000000000001
#define _ALSSIZE			0x0000000000000002
static ff_u64_t flags = 0;
#define isset(__what) \
	((flags&(__what)) == (__what))
#define isnset(__what) \
	!isset(__what)
void
ff_sysconf_init(void) {
	ffly_mem_set(ffly_sysconf, 0, 256);
	*sysconf_get(loaded) = -1;
	*sysconf_get(db_loaded) = -1;
}

void static
ld_def_max_threads(void) {
	*sysconf_get(max_threads) = DEF_MAX_THREADS;
}

void static
ld_def_alssize(void) {
	*sysconf_get(alssize) = ALLOCA_SSIZE;
}

void ffly_ld_sysconf_def() {
	ld_def_max_threads();
	ld_def_alssize();
}

void static
ld_max_threads(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "max_threads")) != NULL) {
		*sysconf_get(max_threads) = ffly_bole_int_u(p);
		flags |= _MAX_THREADS;
	}
	ffly_fprintf(ffly_log, "max threads: %u\n", *sysconf_get(max_threads));
}

void static
ld_alssize(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "alssize")) != NULL) {
		*sysconf_get(alssize) = ffly_bole_int_u(p);
		flags |= _ALSSIZE;
	}
	ffly_fprintf(ffly_log, "alloca ssize: %u\n", *sysconf_get(alssize));
}

void static
ld_moddir(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "moddir")) != NULL)
		*sysconf_get(moddir) = (char const*)ffly_str_dupe(ffly_bole_str(p));	
}

void static
ld_inidir(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "inidir")) != NULL)
		*sysconf_get(inidir) = (char const*)ffly_str_dupe(ffly_bole_str(p));		
}

void static
ld_modl(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "modl")) != NULL) {
		ffly_printf("module list len: %u\n", ffly_bole_arr_len(p));
		ff_u8_t i = 0, l = ffly_bole_arr_len(p);
		*sysconf_get(modl) = (char const**)__ffly_mem_alloc((l+1)*sizeof(char const*));

		while(i != l) {
			(*sysconf_get(modl))[i] = (char const*)ffly_str_dupe(ffly_bole_str(ffly_bole_arr_elem(p, i)));
			ffly_printf("module: %s\n", ffly_bole_str(ffly_bole_arr_elem(p, i)));
			i++;
		}
		(*sysconf_get(modl))[i] = NULL;
	}
}

void static
ld_inil(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "inil")) != NULL) {
		ffly_printf("init list len: %u\n", ffly_bole_arr_len(p));
		ff_u8_t i = 0, l = ffly_bole_arr_len(p);
		if (!(*sysconf_get(inil) = (char const**)__ffly_mem_alloc((l+1)*sizeof(char const*)))) {
			ffly_printf("error.\n");
			return;
		}

		while(i != l) {
			(*sysconf_get(inil))[i] = (char const*)ffly_str_dupe(ffly_bole_str(ffly_bole_arr_elem(p, i)));
			ffly_printf("init: %s\n", ffly_bole_str(ffly_bole_arr_elem(p, i)));
			i++;
		}
		(*sysconf_get(inil))[i] = NULL;
	}
}

void static
ld_db(void const *__db) {
	void const *ip_addr;
	void const *port;
	void const *enckey;
	void const *user;
	void const *passwd;

	ffly_printf("*db-composition:\n");
	if ((ip_addr = ffly_bole_struc_get(__db, "ip_addr")) != NULL) {
		*sysconf_get(db_ip_addr) = (char const*)ffly_str_dupe(ffly_bole_str(ip_addr));
		ffly_printf("\t-ip addr: %s\n", *sysconf_get(db_ip_addr));
	}

	if ((port = ffly_bole_struc_get(__db, "port")) != NULL) {
		*sysconf_get(db_port) = ffly_bole_16l_u(port);
		ffly_printf("\t-port: %u\n", *sysconf_get(db_port));
	}

	if ((enckey = ffly_bole_struc_get(__db, "enckey")) != NULL) {
		*sysconf_get(db_enckey) = (char const*)ffly_str_dupe(ffly_bole_str(enckey));
		ffly_printf("\t-enckey: %s\n", *sysconf_get(db_enckey));
	}

	if ((user = ffly_bole_struc_get(__db, "user")) != NULL) {
		*sysconf_get(db_user) = (char const*)ffly_str_dupe(ffly_bole_str(user));
		ffly_printf("\t-username: %s\n", *sysconf_get(db_user));
	}

	if ((passwd = ffly_bole_struc_get(__db, "passwd")) != NULL) {
		*sysconf_get(db_passwd) = (char const*)ffly_str_dupe(ffly_bole_str(passwd));
		ffly_printf("\t-password: %s\n", *sysconf_get(db_passwd));
	}
	*sysconf_get(db_loaded) = 0;
}

ff_err_t ffly_ld_sysconf(char const *__path) {
	struct ffly_bole conf;
	ff_err_t err = FFLY_SUCCESS;
	if (_err(err = ffly_bole_init(&conf))) {
		ffly_fprintf(ffly_err, "failed to init config.\n");
		return err;
	}

	if (_err(err = ffly_bole_ld(&conf, __path))) {
		ffly_fprintf(ffly_err, "failed to load config.\n");
		return err;
	}
	
	if (_err(err = ffly_bole_read(&conf))) {
		ffly_fprintf(ffly_err, "failed to read config.\n");
		return err;
	}

	ffbole cf;
	// deposit
	ffly_bole_depos(&conf, &cf);

	void const *version;
	
	version = ffly_bole_get(&cf, "version");
	if (!ffly_bole_is_str(version)) {
		ffly_fprintf(ffly_err, "can't read version as type does not match.\n");
		err = FFLY_FAILURE;
		goto _fail;
	}

	void const *root_dir;
	
	root_dir = ffly_bole_get(&cf, "root_dir");
	if (!ffly_bole_is_str(root_dir)) {

	}

	void const *db;
	if ((db = ffly_bole_get(&cf, "db")) != NULL)
		ld_db(db);

	ld_max_threads(&cf);
	ld_alssize(&cf);
	ld_moddir(&cf);
	ld_inidir(&cf);
	ld_modl(&cf);
	ld_inil(&cf);

	
	*sysconf_get(version) = (char const*)ffly_str_dupe(ffly_bole_str(version));
	*sysconf_get(root_dir) = (char const*)ffly_str_dupe(ffly_bole_str(root_dir));
	*sysconf_get(loaded) = 0;

	if (isnset(_MAX_THREADS)) {
		ffly_printf("'max threads' not specified in config, loading default.\n");
		ld_def_max_threads();
	}

	if (isnset(_ALSSIZE)) {
		ffly_printf("'alloca stack size' not specified in config, loading default.\n");
		ld_def_alssize();
	}
_fail:
	if (_err(err = ffly_bole_free(&conf))) {
		ffly_fprintf(ffly_err, "failed to free config:0.\n");
		return err;
	}

	if (_err(err = ffbole_free(&cf))) {
		ffly_fprintf(ffly_err, "failed to free config:1.\n");
		return err;
	}

	return FFLY_SUCCESS;
}

# include "../macros.h"
void ffly_free_sysconf(void) {
/*
	TODO:
		list of pointers to free
		not this:
*/
	__ffly_finn(*sysconf_get(version));
	__ffly_finn(*sysconf_get(root_dir));
	__ffly_finn(*sysconf_get(moddir));
	__ffly_finn(*sysconf_get(inidir));

	__ffly_finn(*sysconf_get(db_ip_addr));
	__ffly_finn(*sysconf_get(db_enckey));
	__ffly_finn(*sysconf_get(db_user));
	__ffly_finn(*sysconf_get(db_passwd));

	char const **mod;
	if ((mod = *sysconf_get(modl)) != NULL) {
		while(*mod != NULL) {
			__ffly_mem_free((void*)*mod);
			mod++;
		}
		__ffly_mem_free(*sysconf_get(modl));
	}

	char const **ini;	
	if ((ini = *sysconf_get(inil)) != NULL) {
		while(*ini != NULL) {
			__ffly_mem_free((void*)*ini);
			ini++;
		}
		__ffly_mem_free(*sysconf_get(inil));
	}
}
