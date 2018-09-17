# include "config.h"
# include "../bole.h"
# include "errno.h"
# include "err.h"
# include "io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../mode.h"
# define DEF_MAX_THREADS 20
# define ALLOCA_SSIZE 400
struct ffly_sysconf __ffly_sysconf__ = {
	.version = NULL, .root_dir = NULL,
	.moddir = NULL, .inidir = NULL,
	.modl = NULL, .inil = NULL,
	.db = {
		.loaded = -1,
		.ip_addr = NULL,
		.port = 0,
		.enckey = NULL,
		.user = NULL,
		.passwd = NULL
	},
	.loaded = -1
};

void static
ld_def_max_threads(void) {
	__ffly_sysconf__.max_threads = DEF_MAX_THREADS;
}

void static
ld_def_alssize(void) {
	__ffly_sysconf__.alssize = ALLOCA_SSIZE;
}

void ffly_ld_sysconf_def() {
	ld_def_max_threads();
	ld_def_alssize();
}

void static
ld_max_threads(ffbole *__cf) {
	void const *p;
	if (!(p = ffly_bole_get(__cf, "max_threads")))
		ld_def_max_threads();
	else
		__ffly_sysconf__.max_threads = ffly_bole_int_u(p);
	ffly_fprintf(ffly_log, "max threads: %u\n", __ffly_sysconf__.max_threads);
}

void static
ld_alssize(ffbole *__cf) {
	void const *p;
	if (!(p = ffly_bole_get(__cf, "alssize")))
		ld_def_alssize();
	else
		__ffly_sysconf__.alssize = ffly_bole_int_u(p);
	ffly_fprintf(ffly_log, "alloca ssize: %u\n", __ffly_sysconf__.alssize);
}

void static
ld_moddir(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "moddir")) != NULL)
		__ffly_sysconf__.moddir = (char const*)ffly_str_dupe(ffly_bole_str(p));	
}

void static
ld_inidir(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "inidir")) != NULL)
		__ffly_sysconf__.inidir = (char const*)ffly_str_dupe(ffly_bole_str(p));		
}

void static
ld_modl(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "modl")) != NULL) {
		ffly_printf("module list len: %u\n", ffly_bole_arr_len(p));
		ff_u8_t i = 0, l = ffly_bole_arr_len(p);
		__ffly_sysconf__.modl = (char const**)__ffly_mem_alloc((l+1)*sizeof(char const*));

		while(i != l) {
			__ffly_sysconf__.modl[i] = (char const*)ffly_str_dupe(ffly_bole_str(ffly_bole_arr_elem(p, i)));
			ffly_printf("module: %s\n", ffly_bole_str(ffly_bole_arr_elem(p, i)));
			i++;
		}
		__ffly_sysconf__.modl[i] = NULL;
	}
}

void static
ld_inil(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "inil")) != NULL) {
		ffly_printf("init list len: %u\n", ffly_bole_arr_len(p));
		ff_u8_t i = 0, l = ffly_bole_arr_len(p);
		if (!(__ffly_sysconf__.inil = (char const**)__ffly_mem_alloc((l+1)*sizeof(char const*)))) {
			ffly_printf("error.\n");
			return;
		}

		while(i != l) {
			__ffly_sysconf__.inil[i] = (char const*)ffly_str_dupe(ffly_bole_str(ffly_bole_arr_elem(p, i)));
			ffly_printf("init: %s\n", ffly_bole_str(ffly_bole_arr_elem(p, i)));
			i++;
		}
		__ffly_sysconf__.inil[i] = NULL;
	}
}

void static
ld_db(void const *__db) {
	void const *ip_addr;
	void const *port;
	void const *enckey;
	void const *user;
	void const *passwd;

	if ((ip_addr = ffly_bole_struc_get(__db, "ip_addr")) != NULL) {
		__ffly_sysconf__.db.ip_addr =  (char const*)ffly_str_dupe(ffly_bole_str(ip_addr));
		ffly_printf("ip addr: %s\n", __ffly_sysconf__.db.ip_addr);
	} else
		__ffly_sysconf__.db.ip_addr = NULL;

	if ((port = ffly_bole_struc_get(__db, "port")) != NULL) {
		__ffly_sysconf__.db.port = ffly_bole_16l_u(port);
		ffly_printf("port: %u\n", __ffly_sysconf__.db.port);
	}

	if ((enckey = ffly_bole_struc_get(__db, "enckey")) != NULL) {
		__ffly_sysconf__.db.enckey = (char const*)ffly_str_dupe(ffly_bole_str(enckey));
		ffly_printf("enckey: %s\n", __ffly_sysconf__.db.enckey);
	} else
		__ffly_sysconf__.db.enckey = NULL;

	if ((user = ffly_bole_struc_get(__db, "user")) != NULL) {
		__ffly_sysconf__.db.user = (char const*)ffly_str_dupe(ffly_bole_str(user));
		ffly_printf("user: %s\n", __ffly_sysconf__.db.user);
	} else
		__ffly_sysconf__.db.user = NULL;

	if ((passwd = ffly_bole_struc_get(__db, "passwd")) != NULL) {
		__ffly_sysconf__.db.passwd = (char const*)ffly_str_dupe(ffly_bole_str(passwd));
		ffly_printf("password: %s\n", __ffly_sysconf__.db.passwd);
	} else
		__ffly_sysconf__.db.passwd = NULL;
	__ffly_sysconf__.db.loaded = 0;
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
	ffly_bole_depos(&conf, &cf);

	void const *version = ffly_bole_get(&cf, "version");
	if (!ffly_bole_is_str(version)) {
		ffly_fprintf(ffly_err, "can't read version as type does not match.\n");
		err = FFLY_FAILURE;
		goto _fail;
	}

	void const *root_dir = ffly_bole_get(&cf, "root_dir");
	if (!ffly_bole_is_str(root_dir)) {

	}

	void const *db = ffly_bole_get(&cf, "db");
	if (db != NULL)
		ld_db(db);

	ld_max_threads(&cf);
	ld_alssize(&cf);
	ld_moddir(&cf);
	ld_inidir(&cf);
	ld_modl(&cf);
	ld_inil(&cf);

	__ffly_sysconf__.version = (char const*)ffly_str_dupe(ffly_bole_str(version));
	__ffly_sysconf__.root_dir = (char const*)ffly_str_dupe(ffly_bole_str(root_dir));
	__ffly_sysconf__.loaded = 0;
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
void ffly_free_sysconf() {
	__ffly_finn(__ffly_sysconf__.version);
	__ffly_finn(__ffly_sysconf__.root_dir);
	__ffly_finn(__ffly_sysconf__.moddir);
	__ffly_finn(__ffly_sysconf__.inidir);

	__ffly_finn(__ffly_sysconf__.db.ip_addr);
	__ffly_finn(__ffly_sysconf__.db.enckey);
	__ffly_finn(__ffly_sysconf__.db.user);
	__ffly_finn(__ffly_sysconf__.db.passwd);

	char const **mod;
	if ((mod = __ffly_sysconf__.modl) != NULL) {
		while(*mod != NULL) {
			__ffly_mem_free((void*)*mod);
			mod++;
		}
		__ffly_mem_free(__ffly_sysconf__.modl);
	}

	char const **ini;	
	if ((ini = __ffly_sysconf__.inil) != NULL) {
		while(*ini != NULL) {
			__ffly_mem_free((void*)*ini);
			ini++;
		}
		__ffly_mem_free(__ffly_sysconf__.inil);
	}
}
